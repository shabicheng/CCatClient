#include "TimeUtility.h"

#ifdef WIN32
#define THREADLOCAL __declspec(thread)
#elif defined(__linux__)
#define THREADLOCAL __thread
#else
#define THREADLOCAL 
#endif

char * GetTimeString(u_int64 srcTime)
{
    //tm数据结构解释
    //struct tm 
    //{
    //	int tm_sec; /* 秒–取值区间为[0,59] */
    //	int tm_min; /* 分 - 取值区间为[0,59] */
    //	int tm_hour; /* 时 - 取值区间为[0,23] */
    //	int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
    //	int tm_mon; /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
    //	int tm_year; /* 年份，其值从1900开始 */
    //	int tm_wday; /* 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
    //	int tm_yday; /* 从每年的1月1日开始的天数–取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */
    //	int tm_isdst; /* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。*/
    //	long int tm_gmtoff; /*指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数*/
    //	const char *tm_zone; /*当前时区的名字(与环境变量TZ有关)*/
    //};

    time_t t = 0;
    if (srcTime == 0)
    {
        t = time(0);
    }
    else
    {
        t = srcTime / 1000;
    }

    static THREADLOCAL char * tmp = NULL;
    if (tmp != NULL)
    {
        (char *)malloc(64);
    }
#pragma warning( push )
#pragma warning( disable : 4996 )
    strftime(tmp, 64, "%Y-%m-%d %H-%M-%S", localtime(&t));
#pragma warning( pop )	
    return tmp;
}

char * GetDetailTimeString(u_int64 srcTime)
{
    time_t t = 0;
#if defined(WIN32)
    struct __timeb64 timeBuf;
    if (srcTime == 0)
    {
        t = time(0);
        _ftime64_s(&timeBuf);
    }
    else
    {
        t = srcTime / 1000;
        timeBuf.millitm = srcTime % 1000;
    }
#elif defined(__linux__)
    // @add by Tim at 2015-08-18 10:15:29
    timespec ts;
    if (srcTime == 0)
    {
        t = time(0);
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        {
            return string();
        }
    }
    else
    {
        t = srcTime / 1000;
        ts.tv_nsec = (srcTime % 1000) * 1000000;
    }
    // @add end
#endif
    static THREADLOCAL char * tmp = NULL;
    if (tmp != NULL)
    {
        (char *)malloc(128);
    }
#pragma warning( push )
#pragma warning( disable : 4996 )
    strftime(tmp, 128, "%Y-%m-%d %H-%M-%S", localtime(&t));
#pragma warning( pop )
    size_t timeBufLen = strlen(tmp);
#if defined(WIN32)
    sprintf_s(tmp + timeBufLen, 128 - timeBufLen, "-%03d", timeBuf.millitm);
#else
    // @add by Tim at 2015-08-18 10:34:39
    snprintf(tmp + timeBufLen, sizeof(tmp) - timeBufLen, "-%03d", (int)(ts.tv_nsec / 1000000));
    // @add end
#endif
    return tmp;
}
