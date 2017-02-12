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
    //tmÊý¾Ý½á¹¹½âÊÍ
    //struct tm 
    //{
    //	int tm_sec; /* Ãë¨CÈ¡ÖµÇø¼äÎª[0,59] */
    //	int tm_min; /* ·Ö - È¡ÖµÇø¼äÎª[0,59] */
    //	int tm_hour; /* Ê± - È¡ÖµÇø¼äÎª[0,23] */
    //	int tm_mday; /* Ò»¸öÔÂÖÐµÄÈÕÆÚ - È¡ÖµÇø¼äÎª[1,31] */
    //	int tm_mon; /* ÔÂ·Ý£¨´ÓÒ»ÔÂ¿ªÊ¼£¬0´ú±íÒ»ÔÂ£© - È¡ÖµÇø¼äÎª[0,11] */
    //	int tm_year; /* Äê·Ý£¬ÆäÖµ´Ó1900¿ªÊ¼ */
    //	int tm_wday; /* ÐÇÆÚ¨CÈ¡ÖµÇø¼äÎª[0,6]£¬ÆäÖÐ0´ú±íÐÇÆÚÌì£¬1´ú±íÐÇÆÚÒ»£¬ÒÔ´ËÀàÍÆ */
    //	int tm_yday; /* ´ÓÃ¿ÄêµÄ1ÔÂ1ÈÕ¿ªÊ¼µÄÌìÊý¨CÈ¡ÖµÇø¼äÎª[0,365]£¬ÆäÖÐ0´ú±í1ÔÂ1ÈÕ£¬1´ú±í1ÔÂ2ÈÕ£¬ÒÔ´ËÀàÍÆ */
    //	int tm_isdst; /* ÏÄÁîÊ±±êÊ¶·û£¬ÊµÐÐÏÄÁîÊ±µÄÊ±ºò£¬tm_isdstÎªÕý¡£²»ÊµÐÐÏÄÁîÊ±µÄ½øºò£¬tm_isdstÎª0£»²»ÁË½âÇé¿öÊ±£¬tm_isdst()Îª¸º¡£*/
    //	long int tm_gmtoff; /*Ö¸¶¨ÁËÈÕÆÚ±ä¸üÏß¶«ÃæÊ±ÇøÖÐUTC¶«²¿Ê±ÇøÕýÃëÊý»òUTCÎ÷²¿Ê±ÇøµÄ¸ºÃëÊý*/
    //	const char *tm_zone; /*µ±Ç°Ê±ÇøµÄÃû×Ö(Óë»·¾³±äÁ¿TZÓÐ¹Ø)*/
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
    if (tmp == NULL)
    {
        tmp = (char *)malloc(128);
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
    struct timespec ts;
    if (srcTime == 0)
    {
        t = time(0);
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        {
            return NULL;
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
    if (tmp == NULL)
    {
        tmp = (char *)malloc(128);
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

char * GetCatTimeString(u_int64 srcTime)
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
    struct timespec ts;
    if (srcTime == 0)
    {
        t = time(0);
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        {
            return NULL;
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
    if (tmp == NULL)
    {
        tmp = (char *)malloc(128);
    }
#pragma warning( push )
#pragma warning( disable : 4996 )
    strftime(tmp, 128, "%Y-%m-%d %H:%M:%S", localtime(&t));
#pragma warning( pop )
    size_t timeBufLen = strlen(tmp);
#if defined(WIN32)
    sprintf_s(tmp + timeBufLen, 128 - timeBufLen, ".%03d", timeBuf.millitm);
#else
    // @add by Tim at 2015-08-18 10:34:39
    snprintf(tmp + timeBufLen, sizeof(tmp) - timeBufLen, "-%03d", (int)(ts.tv_nsec / 1000000));
    // @add end
#endif
    return tmp;

}
