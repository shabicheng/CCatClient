#include "CLog.h"
#include "TimeUtility.h"

typedef struct _CLogInner
{
    /** @brief    �ļ���Ϣ���кš��ļ������������ȣ�. */
    char m_cpp_fileinfo[256];
    
    /** @brief    The file ptr. */
    FILE * m_f_logOut;
}CLogInner;



//��������ģʽ(���������кţ��ļ����� ����)
int g_log_debug = 0;


//�����Ҫ������־�����ڶ���g_log_save_fileName

// ���WIN����Ч
#ifdef NDEBUG
int g_log_saveFlag = 0;
#else
int g_log_saveFlag = 1;
#endif

//�������־�ļ���ʱ��Ϊ��β��������ֹ�����־�ļ��ظ����ù۲�
int g_log_file_withtime = 0;
// ��־���ѡ�ֻ������ѡ���е���־�Żᱻ�����Ĭ��ȫ��
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
    // �������������п��ܳ�ʼ�����
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
                printf("��־�ļ�������������дģʽ��.\n");
            }
        }
        CLog_m_log(CLOG_INFO, "�����־�Ѿ�������");
    }
    ZRCS_LEAVE(g_logCritSection);
}

// CLog::~CLog()
// {
// 	_m_log(CINFO, "�����־ϵͳ�ر�!");
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
	//ֻ�����ļ�������ǰ���·��ȥ��
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
		//���汣֤���255�� ���Բ���Ҫ��strncpy
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
		//���汣֤���255�� ���Բ���Ҫ��strncpy
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
            printf("��־�ļ�������������дģʽ��.\n");
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