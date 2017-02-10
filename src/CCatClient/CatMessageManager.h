
#ifndef CATMESSAGEMANAGER_h__
#define CATMESSAGEMANAGER_h__

#include "CatMessage.h"
#include "CatRootMessage.h"
#include "CatTransaction.h"

typedef struct _CatMessageManager
{
	sds m_domain;
	sds m_hostname;
	sds m_ip;
	long m_throttleTimes;
}CatMessageManager;

/**********************************************************************************************//**
 * add msg
 *
 * @author	ZRZC
 * @date	2017/2/10
 *
 * @param [in,out]	message	If non-null, the message.
 **************************************************************************************************/
void catMessageManagerAdd(CatMessage * message);

/**********************************************************************************************//**
 * end trans
 *
 * @author	ZRZC
 * @date	2017/2/10
 *
 * @param [in,out]	message	If non-null, the message.
 **************************************************************************************************/
void catMessageManagerEndTrans(CatTranscation * trans);

void catMessageManagerFlush(CatRootMessage * rootMsg);

void catMessageManagerInitialize();

void catMessageManagerStartTrans(CatTranscation * trans);



#endif//CATMESSAGEMANAGER_h__
