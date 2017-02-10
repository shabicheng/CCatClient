#include "CatRootMessage.h"

CatRootMessage * copyCatRootMessage(CatRootMessage * pRootMsg)
{
    CatRootMessage * pCopyMsg = (CatRootMessage *)malloc(sizeof(CatRootMessage));
    if (pCopyMsg == NULL)
    {
        return pCopyMsg;
    }
    if (pRootMsg == NULL)
    {
        memset(pCopyMsg, 0, sizeof(CatRootMessage));
    }
    else
    {
        pCopyMsg->m_rootMsg = pRootMsg->m_rootMsg;
        pCopyMsg->m_domain = sdsdup(pRootMsg->m_domain);
        pCopyMsg->m_hostName = sdsdup(pRootMsg->m_hostName);
        pCopyMsg->m_ipAddress = sdsdup(pRootMsg->m_ipAddress);
        pCopyMsg->m_message = sdsdup(pRootMsg->m_message);
        pCopyMsg->m_messageId = sdsdup(pRootMsg->m_messageId);
        pCopyMsg->m_parentMessageId = sdsdup(pRootMsg->m_parentMessageId);
        pCopyMsg->m_rootMessageId = sdsdup(pRootMsg->m_rootMessageId);
        pCopyMsg->m_sessionToken = sdsdup(pRootMsg->m_sessionToken);
        pCopyMsg->m_threadGroupName = sdsdup(pRootMsg->m_threadGroupName);
        pCopyMsg->m_threadId = sdsdup(pRootMsg->m_threadId);
        pCopyMsg->m_threadName = sdsdup(pRootMsg->m_threadName);
    }
    return pCopyMsg;
}

