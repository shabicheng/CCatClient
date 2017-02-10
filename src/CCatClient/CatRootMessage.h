
#ifndef CATROOTMESSAGE_h__
#define CATROOTMESSAGE_h__

#include "CatMessage.h"
#include "sds.h"


typedef struct _CatRootMessage
{
    CatMessage * m_rootMsg;
    sds m_domain;
    sds m_hostName;
    sds m_ipAddress;
    sds m_message;
    sds m_messageId;
    sds m_parentMessageId;
    sds m_rootMessageId;
    sds m_sessionToken;
    sds m_threadGroupName;
    sds m_threadId;
    sds m_threadName;
}CatRootMessage;

CatRootMessage * copyCatRootMessage(CatRootMessage * pRootMsg);

#endif//CATROOTMESSAGE_h__
