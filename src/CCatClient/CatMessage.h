
#ifndef CATMESSAGE_h__
#define CATMESSAGE_h__

#include "sds.h"
#include <string.h>
#include <stdlib.h>
#include "CatClientCommon.h"

typedef struct _CatMessage CatMessage;

// @todo for wyp
#define CatMessageType_Trans 'T'
#define CatMessageType_Event 'E'
#define CatMessageType_HeartBeat 'H'
#define CatMessageType_Trace 'A'
#define CatMessageType_Metric 'M'

typedef struct _CatMessageInner
{
	char m_msgType;
	char m_unused[3]; //4×Ö½Ú¶ÔÆë
	sds m_type;
	sds m_name;
	sds m_status;
	sds m_data;
	unsigned long long m_timeStampMs; //Í³Ò»º¬Òå£¬sec*1000 + ms
	int m_completeFlag;
	void(*setCompleteFlag)  (CatMessage* message, int completeFlag);
	void *(*clear)   (CatMessage* message); // ×¢Òâ clear·µ»ØÖµÊÇmallocµÄÊ×µØÖ·


}CatMessageInner;


struct _CatMessage
{
	void(*addDataPair)   (CatMessage* message, const char *data);
	void(*addData)   (CatMessage* message, const char *dataKey, const char * dataValue);
	void(*setStatus) (CatMessage* message, const char *status);
	void(*setComplete)  (CatMessage* message);
	void *(*clear)   (CatMessage* message); // ×¢Òâ clear·µ»ØÖµÊÇmallocµÄÊ×µØÖ·
};


#define getInnerMsg(pMsg) ((CatMessageInner*)(((char*)(pMsg)) - sizeof(CatMessageInner)))

/**********************************************************************************************//**
 * Deletes the message described by pMsg.
 *
 * @author	ZRZC
 * @date	2017/2/10
 *
 * @param [in,out]	pMsg	If non-null, the message.
 **************************************************************************************************/
static inline void deleteCatMessage(CatMessage * pMsg)
{
    void * pBuf = pMsg->clear(pMsg);
    free(pBuf);
}

static inline int isCatMessageComplete(CatMessage* message)
{
    CatMessageInner * pInner = getInnerMsg(message);
    return pInner->m_completeFlag;
}

static inline unsigned long long getCatMessageTimeStamp(CatMessage* message)
{
    CatMessageInner * pInner = getInnerMsg(message);
    return pInner->m_timeStampMs;
}

static inline void setCatMessageTimeStamp(CatMessage* message, unsigned long long timeMs)
{
    CatMessageInner * pInner = getInnerMsg(message);
    pInner->m_timeStampMs = timeMs;
}


static inline sds getCatMessageType(CatMessage* message)
{
    CatMessageInner * pInner = getInnerMsg(message);
    return pInner->m_type;
}

/**********************************************************************************************//**
 * msgÃ»ÓÐcreate·½·¨£¬Ö»ÓÐ³õÊ¼»¯·½·¨£¬ÇÒÖ»ÄÜ±»ÆäËû×ÓÀàµ÷ÓÃ.
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


#endif//CATMESSAGE_h__
