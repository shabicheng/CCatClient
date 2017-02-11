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
    //tm���ݽṹ����
    //struct tm 
    //{
    //	int tm_sec; /* ��Cȡֵ����Ϊ[0,59] */
    //	int tm_min; /* �� - ȡֵ����Ϊ[0,59] */
    //	int tm_hour; /* ʱ - ȡֵ����Ϊ[0,23] */
    //	int tm_mday; /* һ�����е����� - ȡֵ����Ϊ[1,31] */
    //	int tm_mon; /* �·ݣ���һ�¿�ʼ��0����һ�£� - ȡֵ����Ϊ[0,11] */
    //	int tm_year; /* ��ݣ���ֵ��1900��ʼ */
    //	int tm_wday; /* ���ڨCȡֵ����Ϊ[0,6]������0���������죬1��������һ���Դ����� */
    //	int tm_yday; /* ��ÿ���1��1�տ�ʼ�������Cȡֵ����Ϊ[0,365]������0����1��1�գ�1����1��2�գ��Դ����� */
    //	int tm_isdst; /* ����ʱ��ʶ����ʵ������ʱ��ʱ��tm_isdstΪ������ʵ������ʱ�Ľ���tm_isdstΪ0�����˽����ʱ��tm_isdst()Ϊ����*/
    //	long int tm_gmtoff; /*ָ�������ڱ���߶���ʱ����UTC����ʱ����������UTC����ʱ���ĸ�����*/
    //	const char *tm_zone; /*��ǰʱ��������(�뻷������TZ�й�)*/
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
