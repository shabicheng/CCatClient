/************************************************************************/
/*
author  : ZC
purpose : 日志类
*/
/************************************************************************/
#ifndef CLOG_H
#define CLOG_H

#include "ZRMultiThreadUtility.h"


/// *************************************************************************************************
/// * @enum     CLogType
/// * 
/// * @ingroup  
/// * 
/// * @brief    日志类型
/// * 
/// * @details  
/// * 
/// * @todo     
/// * 
/// * @author   ZRZC
/// * 
/// * @date     2015/11/20 14:43:20
/// **************************************************************************************************/
#define CLOG_INFO   0x01
#define CLOG_WARNING 0x02
#define CLOG_ERROR 0x04
#define CLOG_DEBUG 0x07
#define CLOG_ALL 0x0F



/**********************************************************************************************//**
 * @brief   日志接口，使用宏展开，这样可以获取到文件名函数名和行号，便于调试
 * 			 
 * @detail  这个宏用户可以理解为CLog::Log的展开，可以填入两种参数 (const char *lpszFormat, ...)和
 * 			(CLogType type, const char *lpszFormat, ...)
 * 			后面的参数类似于printf的参数，比如
 * 			INNER_LOG(CERROR, "%s %d", outStr, outValue);
 * 			最后会打印出类似于"[ERROR] 2015.11.20 15.15.16.235 : str 123"的字符串并且会自动换行
 * 			！！！！！特别需要注意的是，这个宏相当于两个语句，所以用户在调用的时候千万不能放在if while的语句里面
 * 			而且if while 语句没有大括号，例如
 * 			if (false)
 * 			    INNER_LOG("can not print.");
 * 			这句代码会直接打印出 can not print.
 * 			因为其展开成了
 * 			
 * 			if (false)
 * 			    if (G_INNER_DEBUG)
 * 			    {
 * 					CLog::_initLocation(__FILE__, __LINE__, __FUNCTION__);
 * 			    }
 * 			CLog::log(...)
 * 			
 * 			这也从侧面说明：编码规范要求，if无论几句话都要加括号！！！！
 * 			注意里面隐藏了一个高级功能，每次在写log的时候，log都会使用msgbus发送一条数据
 * 			
 * @author	ZRZC
 * @date	2015/11/20
 * 
 * @param	G_INNER_DEBUG	The inner debug.
 **************************************************************************************************/
extern int g_log_debug;
#define INNER_LOG \
if (g_log_debug)\
{\
CLog_initLocation(__FILE__, __LINE__, __FUNCTION__);\
}\
CLogLog

/// *************************************************************************************************
/// * @class    CLog
/// * 
/// * @ingroup  
/// * 
/// * @brief    日志类，用于程序员使用，具体使用请调用INNER_LOG宏
/// * 
/// * @details  可以打印出日志所在的文件、函数、行号等信息，便于调试
/// *           同时日志还可以存储问文件，便于后期分析
/// * 
/// * @todo     
/// * 
/// * @author   ZRZC
/// * 
/// * @date     2015/11/20 15:07:22
/// **************************************************************************************************/


	/**********************************************************************************************//**
	 * @brief   Log 输出接口，输入log类型（默认为info）以及输出的日志参数（参数类型与printf参数一致）
	 *
	 * @author	ZRZC
	 * @date	2015/11/20
	 *
	 * @param	lpszFormat	Describes the format to use.
	 * @param	...		  	The ...
	 **************************************************************************************************/
	void CLogLogInfo(const char *lpszFormat, ...);

	/**********************************************************************************************//**
	 * @brief   Log 输出接口，输入log类型以及输出的日志参数（参数类型与printf参数一致）
	 *
	 * @author	ZRZC
	 * @date	2015/11/20
	 *
	 * @param	type	  	log类型
	 * @param	lpszFormat	Describes the format to use.
	 * @param	...		  	具体的变长参数
	 **************************************************************************************************/
	void CLogLog(unsigned char type, const char *lpszFormat, ...);

	/**********************************************************************************************//**
	 * @brief   初始化location，解析文件、行号、函数名等
	 *
	 * @author	ZRZC
	 * @date	2015/11/20
	 *
	 * @param	filepath		The filepath.
	 * @param	line			The line.
	 * @param	callfunction	The callfunction.
	 **************************************************************************************************/
	void CLog_initLocation(const char *filepath, int line, const char *callfunction);


	
	


#endif // CLOG_H
