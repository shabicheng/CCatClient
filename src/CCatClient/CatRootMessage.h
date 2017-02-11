
#ifndef CATROOTMESSAGE_h__
#define CATROOTMESSAGE_h__

#include "CatMessage.h"
#include "sds.h"


typedef struct _CatRootMessage
{
    CatMessage * m_rootMsg;
    sds m_messageId;
    sds m_parentMessageId;
    sds m_rootMessageId;
    sds m_sessionToken;
    sds m_threadGroupName;
    sds m_threadId;
    sds m_threadName;
}CatRootMessage;

CatRootMessage * createCatRootMessage();
CatRootMessage * copyCatRootMessage(CatRootMessage * pRootMsg);

void clearCatRootMessage(CatRootMessage * pRootMsg);
void deleteCatRootMessage(CatRootMessage * pRootMsg);

#endif//CATROOTMESSAGE_h__
