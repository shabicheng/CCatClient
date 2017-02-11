#include "CLog.h"
#include "TimeUtility.h"

typedef struct _CLogInner
{
    /** @brief    文件信息（行号、文件名、函数名等）. */
    char m_cpp_fileinfo[256];
    
    /** @brief    The file ptr. */
    FILE * m_f_logOut;
}CLogInner;



//开启调试模式(输出输出的行号，文件名， 函数)
int g_log_debug = 0;


//如果需要保存日志必须在定义g_log_save_fileName

// 这个WIN下有效
#ifdef NDEBUG
int g_log_saveFlag = 0;
#else
int g_log_saveFlag = 1;
#endif

//保存的日志文件以时间为结尾命名，防止多个日志文件重复不好观察
int g_log_file_withtime = 0;
// 日志输出选项，只有在这选项中的日志才会被输出，默认全开
unsigned char g_log_permissionOpt = 0xFF;

char *g_log_save_fileName = "InnerLog";


static CLogInner * g_innerLog = NULL;
static ZRCRITICALSECTION g_logCritSection = NULL;



char CDEBUG_STR[16] = "[Debug]: ";
char CINFO_STR[16] = "[Info]: ";
char CWARNING_STR[16] = "[Warning]: ";
char CERROR_STR[16] = "[Error]: ";
char CUNKNOWN_STR[16] = "[Unknown]: ";

void CLogGetLocalTime(char tmp[], int size);

static void CLog_m_log(unsigned char type, const char *buf);

void CLogInit()
{
    // 这会存在隐患，有可能初始化多次
    g_logCritSection = ZRCreateCriticalSection();
    ZRCS_ENTER(g_logCritSection);
    if (g_innerLog == NULL)
    {
        g_innerLog = (CLogInner *)malloc(sizeof(CLogInner));
        if (g_log_saveFlag)
        {
            char logName[256] = { '\0' };
            strncat(logName, g_log_save_fileName, 128);
            if (g_log_file_withtime)
            {
                strncat(logName, GetDetailTimeString(0), 64);
            }
            strncat(logName, ".innerLog", 64);
            g_innerLog->m_f_logOut = fopen(logName, "a+");//open(logName, std::ios::app);
            if (g_innerLog->m_f_logOut == NULL)
            {
                printf("日志文件被其他进程以写模式打开.\n");
            }
        }
        CLog_m_log(CLOG_INFO, "监控日志已经建立！");
    }
    ZRCS_LEAVE(g_logCritSection);
}

// CLog::~CLog()
// {
// 	_m_log(CINFO, "监控日志系统关闭!");
// 	if (G_INNER_LOG_SAVE && m_f_logOut != NULL)
// 	{
// 		fclose(m_f_logOut);
// 		m_f_logOut = NULL;
// 	}
// 	ZRDeleteCriticalSection(m_logCritSection);
// }

void CLog_initLocation(const char *filepath, int line, const char *callfunction)
{
	if (g_innerLog == NULL)
	{	
        CLogInit();
    }
	ZRCS_ENTER(g_logCritSection);
	//只保留文件名，把前面的路径去掉
	const char *last = filepath;
	if ((last = strrchr(filepath,'\\')) == NULL && (last = strrchr(filepath,'/')) == NULL)
	{
		last = filepath;
	}
	else
	{
		++last;
	}
	snprintf(g_innerLog->m_cpp_fileinfo, 255, "%s line : %u %s\n--> ", last, line, callfunction);
	ZRCS_LEAVE(g_logCritSection);
	return;
}

void CLogLogInfo(const char *lpszFormat, ...)
{
    if (!(CLOG_INFO & g_log_permissionOpt))
    {
        return;
    }
    if (g_innerLog == NULL)
    {
        CLogInit();
    }
	char szBuffer[1024];
	int maxlen = 1023;
	char *printBuf = szBuffer;
	
	if (g_log_debug)
	{
		ZRCS_ENTER(g_logCritSection);
		//上面保证最多255， 所以不需要用strncpy
		strcpy(szBuffer, g_innerLog->m_cpp_fileinfo);
		int len = (int)strlen(szBuffer);
		printBuf += len;
		maxlen -= len;
		ZRCS_LEAVE(g_logCritSection);
	}

	va_list args;
	va_start(args, lpszFormat);
	vsnprintf(printBuf, maxlen, lpszFormat, args);
	va_end(args);
	szBuffer[maxlen] = '\0';
    CLog_m_log(CLOG_INFO, szBuffer);
}

void CLogLog(unsigned char type, const char *lpszFormat, ...)
{
    if (!(type & g_log_permissionOpt))
    {
        return;
    }
    if (g_innerLog == NULL)
    {
        CLogInit();
    }
	char szBuffer[1024];
	int maxlen = 1023;
	char *printBuf = szBuffer;

	if (g_log_debug)
	{
		ZRCS_ENTER(g_logCritSection);
		//上面保证最多255， 所以不需要用strncpy
		strcpy(szBuffer, g_innerLog->m_cpp_fileinfo);
		int len = (int)strlen(szBuffer);
		printBuf += len;
		maxlen -= len;
		ZRCS_LEAVE(g_logCritSection);
	}

	va_list args;
	va_start(args, lpszFormat);
	vsnprintf(printBuf, maxlen, lpszFormat, args);
	va_end(args);
	szBuffer[maxlen] = '\0';
	CLog_m_log(type, szBuffer);
}



void CLogGetLocalTime( char tmp[], int size )
{
	time_t t = time(0);
	strftime(tmp, size, "%Y-%m-%d %X ", localtime(&t));
}

static void CLog_m_log(unsigned char type, const char *buf)
{
    char tmpTime[64] = { 0 };
    const char *tmpType = NULL;
    char tmpBuf[1024 + 128] = { 0 };
    CLogGetLocalTime(tmpTime, sizeof(tmpTime));
    ZRCS_ENTER(g_logCritSection);

    switch (type)
    {
    case CLOG_DEBUG:
        //ZRMessageBus::SendData(DEBUG_LOG_NO, buf, 8);
        tmpType = CDEBUG_STR;
        snprintf(tmpBuf, 1024 + 128, "%s%s%s\n", tmpTime, tmpType, buf);
        printf(tmpBuf);
        if (g_log_debug)
        {
            //qDebug(tmpBuf);
        }
        
        break;
    case CLOG_INFO:
        //ZRMessageBus::SendData(INFO_LOG_NO, buf, 8);
        tmpType = CINFO_STR;
        snprintf(tmpBuf, 1024 + 128, "%s%s%s\n", tmpTime, tmpType, buf);
        printf(tmpBuf);
        if (g_log_debug)
        {
            //qDebug(tmpBuf);
        }
        break;
    case CLOG_WARNING:
        //ZRMessageBus::SendData(WARNING_LOG_NO, buf, 8);
        tmpType = CWARNING_STR;
        snprintf(tmpBuf, 1024 + 128, "%s%s%s\n", tmpTime, tmpType, buf);
        printf(tmpBuf);
        if (g_log_debug)
        {
            //qDebug(tmpBuf);
        }
        
        break;
    case CLOG_ERROR:
        //ZRMessageBus::SendData(ERROR_LOG_NO, buf, 8);
        tmpType = CERROR_STR;
        snprintf(tmpBuf, 1024 + 128, "%s%s%s\n", tmpTime, tmpType, buf);
        printf(tmpBuf);
        if (g_log_debug)
        {
            //qDebug(tmpBuf);
        }
        break;
    default:
        tmpType = CUNKNOWN_STR;
        snprintf(tmpBuf, 1024 + 128, "%s%s%s\n", tmpTime, tmpType, buf);
        printf(tmpBuf);
        break;
    }

    if (g_log_saveFlag && g_innerLog->m_f_logOut == NULL)
    {
        g_innerLog->m_f_logOut = fopen(g_log_save_fileName, "a+");
        if (g_innerLog->m_f_logOut != NULL)
        {
            printf("日志文件被其他进程以写模式打开.\n");
        }
    }
    ZRCS_LEAVE(g_logCritSection);
    if (g_log_saveFlag)
    {
        if (g_innerLog->m_f_logOut != NULL)
        {
            fputs(tmpBuf, g_innerLog->m_f_logOut);
            fflush(g_innerLog->m_f_logOut);
        }
    }
}