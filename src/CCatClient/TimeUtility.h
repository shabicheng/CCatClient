// /*************************************************************************************************
// @Assembly         
// @Author           ZRZC
// @Created          09-01-2015
//
// @Last Modified By ZRZC
// @Last Modified On 11-19-2015
// 
// @copyright        Copyright (c) ZhenRong. All rights reserved.
// @summary          时间操作的一些公共类，我们把时间表示为64位整形，并提供一些操作函数
//                   在这里面还提供了高精度计时的一些函数，多用于测试代码运行时间
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
Description: 获取系统64位时间，时间/1000为seconds since midnight, January 1, 1970(UTC)
			 时间 %1000 为毫秒
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
Description: 获取两个时间区间长度，单位毫秒
Input:		 u_int64 oldTime 靠前的时间, u_int64 newTime 靠后的时间
Return:		 u_int64
***********************************/
int64 inline GetTimeIntervalLength64(u_int64 oldTime, u_int64 newTime)
{
	return newTime - oldTime;
}

/**********************************
Function:	 SetNowTime
Description: 设置系统当前时间
Input:		 u_int64 nowTime
Return:		 bool
***********************************/
int inline SetNowTime(u_int64 nowTime)
{
#ifdef WIN32
	//更新本地系统时间
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
		printf("Current time:%u年 %u月 %u日 %u时 %u分 %u秒 %u毫秒\n",
			curr_st.wYear, curr_st.wMonth, curr_st.wDay, curr_st.wHour, curr_st.wMinute,
			curr_st.wSecond, curr_st.wMilliseconds);
		return 1;
	}

#endif
}

/**********************************
Function:	 GetTimeIncrementLength64
Description: 计算时间增量，增加的单位为ms
Input:		 u_int64 srcTime, u_int64 incTime
Return:		 u_int64
***********************************/
u_int64 inline GetTimeIncrementLength64(u_int64 srcTime, int64 incTime)
{
	return srcTime + incTime;
}



/**********************************
Function:	 GetTimeString
Description: 获取时间，格式为2013-10-11 23:21:16
Input:		 u_int64 srcTime = 0,如果srcTime为0，则使用当前时间，否则使用srcTime时间
Return:		 string
***********************************/
inline char *  GetTimeString(u_int64 srcTime)
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
Description: 获取时间，格式为2013-10-11 23:21:16
Input:		 u_int64 srcTime = 0,如果srcTime为0，则使用当前时间，否则使用srcTime时间
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
Description: 获取精确定时器频率
Input:		
Return:		 true获得成功 false表示不支持精确定时
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


//时间区间
typedef struct _TimeInterval
{
	u_int64 time_s;
	u_int64 time_e;
}TimeInterval;

/**********************************
Function:	 TimeIntervalCompare
Description: 时间区间的比较，主要是反应des的区间与res重合程度
Input:		 TimeInterval & resInterval, TimeInterval & desInterval
Return:	
Others:      返回值是rst,rst =0表示没有重合，其他返回值参见注释
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