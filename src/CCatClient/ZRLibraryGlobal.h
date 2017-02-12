


#ifndef _ZHENRONG_COMMON_
#define _ZHENRONG_COMMON_

//ÒÔÏÂÎªÒýÓÃµÄ²Ù×÷ÏµÍ³Í·ÎÄ¼þ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>

#ifdef WIN32
#define inline __inline
#include <winsock2.h>
#include <sys/timeb.h>
#include <windows.h>
#include <process.h>

typedef int socklen_t;

#elif defined(__linux__)

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <stdarg.h>
#include <time.h>
#include <ifaddrs.h>
#include <sys/times.h>
#include <signal.h>
#include <ifaddrs.h>

//added on 14-2-19

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/ipc.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <fcntl.h>

#elif defined(_VXWORKS)

#include <vxworks.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include <types.h>
#include <sockLib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <hostLib.h>
#include <ticklib.h>
#include <drv/timer/ppcDecTimer.h>

#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ioctl.h>
#include  <net/if.h>
#include  <net/if_arp.h>
#include <tipc/tipc.h>
#include <taskLib.h>
#include <selectLib.h>//for vx6
#include <ioLib.h>
#include <ioctl.h>

#endif

#include "ZRLibraryGlobal.h"


/**********************************************************************************************//**
* @brief   ÅÐ¶Ï±¾»úÊÇ·ñÎª´óÐ¡¶Ë.
*
* @return	The local little endian.
**************************************************************************************************/
//extern bool ZRLIBRARY_EXPORT LOCAL_LITTLE_ENDIAN;

#if defined WIN32 || defined _VXWORKS

typedef unsigned char u_char;
typedef unsigned char u_int8;
typedef unsigned short u_int16;
typedef unsigned int u_int32;
typedef unsigned __int64 u_int64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed __int64 int64;

#elif defined __linux__

// @add by Tim at 2015-08-17 16:30:13
typedef unsigned char u_char;
typedef unsigned char u_int8;
typedef unsigned short u_int16;
typedef unsigned int u_int32;
typedef unsigned long long u_int64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef long long int64;
// @add end

typedef int SOCKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define __in   // ±íÊ¾¸Ã²ÎÊýÓÃÓÚÊäÈë
#define __out  // ±íÊ¾¸Ã²ÎÊýÓÃÓÚÊä³öÖµ
#define closesocket close
#define stricmp strcasecmp
typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define Sleep(param) usleep(1000*param)
#define strcpy_s(a, b, c) strcpy(a, c)
#define sprintf_s(a, b, c) sprintf(a, c)
#define strncpy_s(a, b, c, d) strncpy(a, c, d)
#define vsprintf_s(a, b, c, d) vsprintf(a, c, d)
#define _strdup strdup
#define _stricmp stricmp



#define NETEWOULDBLOCK EWOULDBLOCK

// »ñÈ¡´íÎóºÅ

#define NetGetLastError() (errno)
#define GetLastError() (errno)

// 
// // »ñÈ¡ipµØÖ·
// // ÓÐÇÒ½öÓÐÔò·µ»Ø127.0.0.1
// // ·µ»Ø×îºóÒ»¸ö²»ÊÇ127.0.0.1µÄip
// // Èç¹ûÅäÖÃipÔÚµØÖ·±í,Ôò·µ»ØÅäÖÃip
// inline bool NetGetLocalIP(char *ip, const char *configIP = NULL)
// {
// 	struct ifaddrs * ifAddrStruct = NULL;
// 	getifaddrs(&ifAddrStruct); // »ñÈ¡µØÖ·ÁÐ±í
// 
// 	bool isFind = false, isSave = false;
// 	strcpy(ip, "127.0.0.1"); // ÎÞÍø¿¨
// 
// 	while (ifAddrStruct != NULL)
// 	{
// 		if (ifAddrStruct->ifa_addr->sa_family == AF_INET)   // check it is IP4
// 		{
// 			void *tmpAddrPtr = NULL;
// 			// is a valid IP4 Address
// 			if (!strcmp(ifAddrStruct->ifa_name, "lo")) // ÊÇ127.0.0.1 ¼ÌÐø±È¶Ô
// 			{
// 				tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
// 				inet_ntop(AF_INET, tmpAddrPtr, ip, INET_ADDRSTRLEN);
// 				isFind = true;
// 				isSave = true;
// 				ifAddrStruct = ifAddrStruct->ifa_next;
// 				continue;
// 			}
// 
// 			tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
// 			inet_ntop(AF_INET, tmpAddrPtr, ip, INET_ADDRSTRLEN);
// 			isFind = true; // ÕÒµ½ÁË·Ç127.0.0.1µÄip
// 
// 			if (configIP != NULL) // ÅäÖÃÁËipµØÖ·Ôò±È½Ï
// 			{
// 				isFind = false;
// 				while (ifAddrStruct != NULL)
// 				{
// 					char tmpip[16];
// 					tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
// 					inet_ntop(AF_INET, tmpAddrPtr, tmpip, INET_ADDRSTRLEN);
// 					if (!strcmp(tmpip, configIP))
// 					{
// 						strcpy(ip, tmpip);
// 						isFind = true;
// 						break;
// 					}
// 					ifAddrStruct = ifAddrStruct->ifa_next;
// 				}
// 			}
// 			return isFind;
// 		}
// 		ifAddrStruct = ifAddrStruct->ifa_next;
// 	}
// 
// 	return isFind;
// }


#endif

/**********************************************************************************************//**
* ÎªÁËÒþ²Ø½Ó¿ÚÀàµÄÊµÏÖ¶øÌá¹©µÄÒ»×éºê¶¨Òå£¬Ä¿µÄÊÇ±ÜÃâÔÚÍ·ÎÄ¼þÖÐ±©Â¶¹ý¶àÏ¸½Ú»ò¹ý¶àÒÀÀµÏî
* ±¾ÖÊÉÏÊÇ½«ÀàµÄÊµÏÖ·ÅÔÚÆäPrivateÀàÖÐ£¬PrivateÀà¶¨ÒåÔÚcppÖÐ
* ¾ßÌåÊ¹ÓÃ·½·¨²Î¼ûZRLibraryTestÖÐµÄCommonIncludeTest.hºÍCommonIncludeTest.cpp
*
* @author	ZRZC
* @date	2016/6/2
*
**************************************************************************************************/
#define ZR_DECLARE_IMPLEMENTATION(Class) \
	Class##Private * d_impl; \
	inline Class##Private* d_func() { return d_impl; } \
	inline const Class##Private* d_func() const { return d_impl; } \
	friend class Class##Private;

#define ZR_IMPLEMENTATION_INIT(Class) \
	d_impl = new Class##Private; \
	d_impl->q_inter = this;


#define ZR_IMPLEMENTATION_DESTROY(Class) \
	d_impl->q_inter = NULL; \
	delete d_impl; \
	d_impl = NULL;

#define ZR_DECLARE_INTERFACE(Class)    \
	Class * q_inter; \
	inline Class* q_func() { return q_inter; } \
	inline const Class* q_func() const { return q_inter; } \
	friend class Class;

#define ZR_D(Class) Class##Private * const d = d_func()
#define ZR_Q(Class) Class * const q = q_func()

/*
Ðè×¢Òâ£¬Ò»¶¨Òª·ÅÔÚprivateÉùÃ÷ÏÂ
Some classes do not permit copies to be made of an object. These
classes contains a private copy constructor and assignment
operator to disable copying (the compiler gives an error message).
*/
#define ZR_DISABLE_COPY(Class) \
	Class(const Class &) = delete; \
	Class &operator=(const Class &) = delete;



#ifdef WIN32
#define snprintf sprintf_s
// WIN32ÌØÊâÓÃ·¨WStringÓëÆÕÍ¨stringµÄ»¥Ïà×ª»»
//ÓÃÍêÖ®ºódelete
/// *************************************************************************************************
/// * @fn       CstrToWstr
/// * @brief    ½«ÆÕÍ¨µÄstring×ª»»Îª¶à×Ö½Ú×Ö·û´®£¬×Ö·û´®Ê¹ÓÃnew·ÖÅä³öÀ´
/// * @param    cstr  The CSTR.
/// * @returns    wchar_t *.
/// * 
/// * @details  ÓÃ»§ÐèÒªÊÖ¶¯deleteµô·µ»ØµÄÖ¸Õë
/// * 
/// * @todo     
/// * 
/// * @author   ZRZC
/// * 
/// * @date     2015/11/20 15:42:24
/// ************************************************************************************************/
// inline wchar_t* CstrToWstr(const string & cstr)
// {
// 	size_t size = cstr.length();
// 	wchar_t *buffer = new wchar_t[size + 1];
// 	MultiByteToWideChar(CP_ACP, 0, cstr.c_str(), (int)size, buffer, (int)size * sizeof(wchar_t));
// 	buffer[size] = 0;  // È·±£ÒÔ '\0' ½áÎ²
// 	return buffer;
// }
// 
// /// *************************************************************************************************
// /// * @fn       WstrToCstr
// /// * @brief    ½«¶à×Ö½Ú×Ö·û´®×ª»»Îªstring
// /// * @param    lpcwszStr  The LPCWSZ string.
// /// * @returns    string.
// /// * 
// /// * @details  
// /// * 
// /// * @todo     
// /// * 
// /// * @author   ZRZC
// /// * 
// /// * @date     2015/11/20 15:43:29
// /// ************************************************************************************************/
// inline string WstrToCstr(const LPCWSTR lpcwszStr)
// {
// 	string str;
// 	DWORD dwMinSize = 0;
// 	LPSTR lpszStr = NULL;
// 	dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, NULL, 0, NULL, FALSE);
// 	if (0 == dwMinSize)
// 	{
// 		return str;
// 	}
// 	lpszStr = new char[dwMinSize];
// 	WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, lpszStr, dwMinSize, NULL, FALSE);
// 	str = lpszStr;
// 	delete[] lpszStr;
// 	return str;
// }
#endif //WIN32


#endif
