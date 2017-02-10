
#ifndef CATMESSAGE_h__
#define CATMESSAGE_h__

#include "sds.h"
#include <string.h>
#include <stdlib.h>

typedef struct _CatMessage CatMessage;

// @todo for wyp
#define CatMessageType_Trans 'T'
#define CatMessageType_Event 'E'
#define CatMessageType_HeartBeat 'H'
#define CatMessageType_Trace 'A'
#define CatMessageType_Metric 'M'

#define CAT_SUCCESS "success"
#define CAT_ERROR "error"
#define CAT_FAIL "fail"

typedef struct _CatMessageInner
{
	char m_msgType;
	char m_unused[3]; //4字节对齐
	sds m_type;
	sds m_name;
	sds m_status;
	sds m_data;
	unsigned long long m_timeStamp; //统一含义，sec*1000 + ms
	int m_completeFlag;
	void(*setCompleteFlag)  (CatMessage* message, int completeFlag);
	void *(*clear)   (CatMessage* message); // 注意 clear返回值是malloc的首地址


}CatMessageInner;


struct _CatMessage
{
	void(*addDataPair)   (CatMessage* message, const char *data);
	void(*addData)   (CatMessage* message, const char *dataKey, const char * dataValue);
	void(*setStatus) (CatMessage* message, const char *status);
	void(*setComplete)  (CatMessage* message);
	void *(*clear)   (CatMessage* message); // 注意 clear返回值是malloc的首地址
};

int isCatMessageComplete(CatMessage* message);
unsigned long long getCatMessageTimeStamp(CatMessage* message);

/**********************************************************************************************//**
 * msg没有create方法，只有初始化方法，且只能被其他子类调用.
 *
 * @author	ZRZC
 * @date	2017/2/10
 *
 * @param [in,out]	pMsg	If non-null, the message.
 * @param	msgType			Type of the message.
 * @param	type			The type.
 * @param	name			The name.
 **************************************************************************************************/
void initCatMessage(CatMessage * pMsg, char msgType, const char *type, const char * name);

/**********************************************************************************************//**
 * Deletes the message described by pMsg.
 *
 * @author	ZRZC
 * @date	2017/2/10
 *
 * @param [in,out]	pMsg	If non-null, the message.
 **************************************************************************************************/
void deleteCatMessage(CatMessage * pMsg);

#define getInnerMsg(pMsg) ((CatMessageInner*)(((char*)(pMsg)) - sizeof(CatMessageInner)))

#endif//CATMESSAGE_h__
