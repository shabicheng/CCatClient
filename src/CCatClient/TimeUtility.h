// /*************************************************************************************************
// @Assembly         
// @Author           ZRZC
// @Created          09-01-2015
//
// @Last Modified By ZRZC
// @Last Modified On 11-19-2015
// 
// @copyright        Copyright (c) ZhenRong. All rights reserved.
// @summary          Ê±¼ä²Ù×÷µÄÒ»Ð©¹«¹²Àà£¬ÎÒÃÇ°ÑÊ±¼ä±íÊ¾Îª64Î»ÕûÐÎ£¬²¢Ìá¹©Ò»Ð©²Ù×÷º¯Êý
//                   ÔÚÕâÀïÃæ»¹Ìá¹©ÁË¸ß¾«¶È¼ÆÊ±µÄÒ»Ð©º¯Êý£¬¶àÓÃÓÚ²âÊÔ´úÂëÔËÐÐÊ±¼ä
// **************************************************************************************************/

#ifndef  _TIMEUTILITY_
#define  _TIMEUTILITY_

#include "ZRLibraryGlobal.h"

#ifdef WIN32
#include <sys/timeb.h>
#endif


/**********************************
Function:	 GetTime64
Description: »ñÈ¡ÏµÍ³64Î»Ê±¼ä£¬Ê±¼ä/1000Îªseconds since midnight, January 1, 1970(UTC)
			 Ê±¼ä %1000 ÎªºÁÃë
Return: u_int64
***********************************/
static u_int64 inline GetTime64()
{
	u_int64 buf;
#if defined(WIN32)
	struct __timeb64 timeBuf;
	_ftime64_s(&timeBuf);
	buf = (timeBuf.time * 1000) + (timeBuf.millitm);
#elif defined(__linux__)
	// @add by Tim at 2015-08-18 10:15:29
	struct timeval tv;
	gettimeofday(&tv, NULL);
	buf = tv.tv_sec * 1000 + tv.tv_usec / 1000000;
	// @add end
#endif
	return buf;
}
/**********************************
Function:	 GetTimeIntervalLength64
Description: »ñÈ¡Á½¸öÊ±¼äÇø¼ä³¤¶È£¬µ¥Î»ºÁÃë
Input:		 u_int64 oldTime ¿¿Ç°µÄÊ±¼ä, u_int64 newTime ¿¿ºóµÄÊ±¼ä
Return:		 u_int64
***********************************/
static int64 inline GetTimeIntervalLength64(u_int64 oldTime, u_int64 newTime)
{
	return newTime - oldTime;
}

/**********************************
Function:	 SetNowTime
Description: ÉèÖÃÏµÍ³µ±Ç°Ê±¼ä
Input:		 u_int64 nowTime
Return:		 bool
***********************************/
static int inline SetNowTime(u_int64 nowTime)
{
#ifdef WIN32
	//¸üÐÂ±¾µØÏµÍ³Ê±¼ä
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
		printf("Current time:%uÄê %uÔÂ %uÈÕ %uÊ± %u·Ö %uÃë %uºÁÃë\n",
			curr_st.wYear, curr_st.wMonth, curr_st.wDay, curr_st.wHour, curr_st.wMinute,
			curr_st.wSecond, curr_st.wMilliseconds);
		return 1;
	}

#endif
}

/**********************************
Function:	 GetTimeIncrementLength64
Description: ¼ÆËãÊ±¼äÔöÁ¿£¬Ôö¼ÓµÄµ¥Î»Îªms
Input:		 u_int64 srcTime, u_int64 incTime
Return:		 u_int64
***********************************/
static u_int64 inline GetTimeIncrementLength64(u_int64 srcTime, int64 incTime)
{
	return srcTime + incTime;
}



/**********************************
Function:	 GetTimeString
Description: »ñÈ¡Ê±¼ä£¬¸ñÊ½Îª2013-10-11 23:21:16
Input:		 u_int64 srcTime = 0,Èç¹ûsrcTimeÎª0£¬ÔòÊ¹ÓÃµ±Ç°Ê±¼ä£¬·ñÔòÊ¹ÓÃsrcTimeÊ±¼ä
Return:		 string
***********************************/
char *  GetTimeString(u_int64 srcTime);

static int inline GetTimeHour(u_int64 timeV)
{
    time_t t = 0;
    struct tm * pTm = NULL;
	if (timeV == 0)
	{
		t = time(0);
	}
	else
	{
		t = timeV / 1000;
	}
    pTm = localtime(&t);
	return pTm->tm_hour;
}

/**********************************
Function:	 GetDetailTimeString
Description: »ñÈ¡Ê±¼ä£¬¸ñÊ½Îª2013-10-11 23:21:16
Input:		 u_int64 srcTime = 0,Èç¹ûsrcTimeÎª0£¬ÔòÊ¹ÓÃµ±Ç°Ê±¼ä£¬·ñÔòÊ¹ÓÃsrcTimeÊ±¼ä
Return:		 string
***********************************/
char *  GetDetailTimeString(u_int64 srcTime);


char * GetCatTimeString(u_int64 srcTime);


/**********************************
Function:	 GetPerformanceFrequency
Description: »ñÈ¡¾«È·¶¨Ê±Æ÷ÆµÂÊ
Input:		
Return:		 true»ñµÃ³É¹¦ false±íÊ¾²»Ö§³Ö¾«È·¶¨Ê±
***********************************/
static int inline GetPerformanceFrequency(int64 * frequence)
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


static int64 inline GetPerformanceCounter()
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


//Ê±¼äÇø¼ä
typedef struct _TimeInterval
{
	u_int64 time_s;
	u_int64 time_e;
}TimeInterval;

/**********************************
Function:	 TimeIntervalCompare
Description: Ê±¼äÇø¼äµÄ±È½Ï£¬Ö÷ÒªÊÇ·´Ó¦desµÄÇø¼äÓëresÖØºÏ³Ì¶È
Input:		 TimeInterval & resInterval, TimeInterval & desInterval
Return:	
Others:      ·µ»ØÖµÊÇrst,rst =0±íÊ¾Ã»ÓÐÖØºÏ£¬ÆäËû·µ»ØÖµ²Î¼û×¢ÊÍ
***********************************/
static inline u_int8 TimeIntervalCompare(TimeInterval * resInterval, TimeInterval  *desInterval)
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