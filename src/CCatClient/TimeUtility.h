// /*************************************************************************************************
// @Assembly         
// @Author           ZRZC
// @Created          09-01-2015
//
// @Last Modified By ZRZC
// @Last Modified On 11-19-2015
// 
// @copyright        Copyright (c) ZhenRong. All rights reserved.
// @summary          ʱ�������һЩ�����࣬���ǰ�ʱ���ʾΪ64λ���Σ����ṩһЩ��������
//                   �������滹�ṩ�˸߾��ȼ�ʱ��һЩ�����������ڲ��Դ�������ʱ��
// **************************************************************************************************/

#ifndef  _TIMEUTILITY_
#define  _TIMEUTILITY_

#include "ZRLibraryGlobal.h"

#ifdef WIN32
#define inline __inline
#include <sys/timeb.h>
#endif


/**********************************
Function:	 GetTime64
Description: ��ȡϵͳ64λʱ�䣬ʱ��/1000Ϊseconds since midnight, January 1, 1970(UTC)
			 ʱ�� %1000 Ϊ����
Return: u_int64
***********************************/
u_int64 inline GetTime64()
{
	u_int64 buf;
#if defined(WIN32)
	struct __timeb64 timeBuf;
	_ftime64_s(&timeBuf);
	buf = (timeBuf.time * 1000) + (timeBuf.millitm);
#elif defined(__linux__)
	// @add by Tim at 2015-08-18 10:15:29
	timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	{
		return (u_int64)-1;
	}
	buf = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	// @add end
#endif
	return buf;
}
/**********************************
Function:	 GetTimeIntervalLength64
Description: ��ȡ����ʱ�����䳤�ȣ���λ����
Input:		 u_int64 oldTime ��ǰ��ʱ��, u_int64 newTime �����ʱ��
Return:		 u_int64
***********************************/
int64 inline GetTimeIntervalLength64(u_int64 oldTime, u_int64 newTime)
{
	return newTime - oldTime;
}

/**********************************
Function:	 SetNowTime
Description: ����ϵͳ��ǰʱ��
Input:		 u_int64 nowTime
Return:		 bool
***********************************/
int inline SetNowTime(u_int64 nowTime)
{
#ifdef WIN32
	//���±���ϵͳʱ��
	SYSTEMTIME curr_st;
	struct tm *local = NULL;
	time_t tt = nowTime / 1000;

#pragma warning(push)
#pragma warning(disable : 4996)

	local = localtime(&tt);
#pragma  warning(pop)

	curr_st.wYear = (WORD)local->tm_year + 1900;
	curr_st.wMonth = (WORD)local->tm_mon + 1;
	curr_st.wDay = (WORD)local->tm_mday;
	curr_st.wHour = (WORD)local->tm_hour;
	curr_st.wMinute = (WORD)local->tm_min;
	curr_st.wSecond = (WORD)local->tm_sec;
	curr_st.wMilliseconds = (WORD)(nowTime % 1000);
	int rst = SetLocalTime(&curr_st);
	if (!rst)
	{
		printf("Set Local Time failed,Error No.: %u", GetLastError());
		return 0;
	}
	else
	{
		printf("Update local time successfully!\n");
		printf("Current time:%u�� %u�� %u�� %uʱ %u�� %u�� %u����\n",
			curr_st.wYear, curr_st.wMonth, curr_st.wDay, curr_st.wHour, curr_st.wMinute,
			curr_st.wSecond, curr_st.wMilliseconds);
		return 1;
	}

#endif
}

/**********************************
Function:	 GetTimeIncrementLength64
Description: ����ʱ�����������ӵĵ�λΪms
Input:		 u_int64 srcTime, u_int64 incTime
Return:		 u_int64
***********************************/
u_int64 inline GetTimeIncrementLength64(u_int64 srcTime, int64 incTime)
{
	return srcTime + incTime;
}



/**********************************
Function:	 GetTimeString
Description: ��ȡʱ�䣬��ʽΪ2013-10-11 23:21:16
Input:		 u_int64 srcTime = 0,���srcTimeΪ0����ʹ�õ�ǰʱ�䣬����ʹ��srcTimeʱ��
Return:		 string
***********************************/
inline char *  GetTimeString(u_int64 srcTime)
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

	char * timestr;
	time_t t = 0;
	if (srcTime == 0)
	{
		t = time(0);
	}
	else
	{
		t = srcTime / 1000;
	}
	char tmp[64];
#pragma warning( push )
#pragma warning( disable : 4996 )
	strftime(tmp, 64, "%Y-%m-%d %H-%M-%S", localtime(&t));
#pragma warning( pop )	
	timestr = strdup(tmp);
	return timestr;
}

int inline GetTimeHour(u_int64 timeV)
{
	time_t t = 0;
	if (timeV == 0)
	{
		t = time(0);
	}
	else
	{
		t = timeV / 1000;
	}
	struct tm * pTm = localtime(&t);
	return pTm->tm_hour;
}

/**********************************
Function:	 GetDetailTimeString
Description: ��ȡʱ�䣬��ʽΪ2013-10-11 23:21:16
Input:		 u_int64 srcTime = 0,���srcTimeΪ0����ʹ�õ�ǰʱ�䣬����ʹ��srcTimeʱ��
Return:		 string
***********************************/
inline char *  GetDetailTimeString(u_int64 srcTime)
{
	char * timestr;
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

	char tmp[128];
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
	timestr = strdup(tmp);
	return timestr;
}


/**********************************
Function:	 GetPerformanceFrequency
Description: ��ȡ��ȷ��ʱ��Ƶ��
Input:		
Return:		 true��óɹ� false��ʾ��֧�־�ȷ��ʱ
***********************************/
int inline GetPerformanceFrequency(int64 * frequence)
{
#ifdef WIN32
	LARGE_INTEGER f;
	if (QueryPerformanceFrequency(&f))
	{
		*frequence = f.QuadPart;
		return 1;
	}
	else
	{
		return 0;
	}
#elif defined(__linux)
	*frequence = 1000000000;
	return 1;
#else

return 0;
#endif //WIN32
}


int64 inline GetPerformanceCounter()
{
#ifdef WIN32
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	return c.QuadPart;
#elif defined(__linux)
	struct timespec timeNow = {0, 0};
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeNow);
	return (int64)timeNow.tv_sec * 1000000000 + timeNow.tv_nsec;
#else

#endif //WIN32
	return 0;
}


//ʱ������
typedef struct _TimeInterval
{
	u_int64 time_s;
	u_int64 time_e;
}TimeInterval;

/**********************************
Function:	 TimeIntervalCompare
Description: ʱ������ıȽϣ���Ҫ�Ƿ�Ӧdes��������res�غϳ̶�
Input:		 TimeInterval & resInterval, TimeInterval & desInterval
Return:	
Others:      ����ֵ��rst,rst =0��ʾû���غϣ���������ֵ�μ�ע��
***********************************/
inline u_int8 TimeIntervalCompare(TimeInterval * resInterval, TimeInterval  *desInterval)
{
	u_int8 rst = 0;
	if (resInterval->time_e < desInterval->time_s)
	{
		rst = 0;
	}
	else
	{	//res.s > des.e no cross
		if (resInterval->time_s > desInterval->time_e)
		{
			rst = 0;
		}
		else
		{
			if (resInterval->time_s > desInterval->time_s)
			{
				if (resInterval->time_e > desInterval->time_e)
				{
					//res                             |________|
					//des                    |__________|
					rst = 1;
				}
				else
				{
					//res                     |__________|
					//des                |_________________|
					rst = 2;
				}
			}
			else
			{
				if (resInterval->time_e > desInterval->time_e)
				{
					//res              |____________|
					//des                  |_______|
					rst = 3;
				}
				else
				{
					//res              |___________|
					//des                 |_____________|
					rst = 4;
				}
			}
		}
	}
	return rst;
}
#endif