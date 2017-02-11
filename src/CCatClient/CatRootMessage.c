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
//         pCopyMsg->m_domain = sdsdup(pRootMsg->m_domain);
//         pCopyMsg->m_hostName = sdsdup(pRootMsg->m_hostName);
//         pCopyMsg->m_ipAddress = sdsdup(pRootMsg->m_ipAddress);
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

void clearCatRootMessage(CatRootMessage * pRootMsg)
{
    pRootMsg->m_rootMsg = NULL;
    if (pRootMsg->m_messageId != NULL)
    {
        sdsfree(pRootMsg->m_messageId);
        pRootMsg->m_messageId = NULL;
    }
    if (pRootMsg->m_parentMessageId != NULL)
    {
        sdsfree(pRootMsg->m_parentMessageId);
        pRootMsg->m_parentMessageId = NULL;
    }
    if (pRootMsg->m_rootMessageId != NULL)
    {
        sdsfree(pRootMsg->m_rootMessageId);
        pRootMsg->m_rootMessageId = NULL;
    }
    if (pRootMsg->m_sessionToken != NULL)
    {
        sdsfree(pRootMsg->m_sessionToken);
        pRootMsg->m_sessionToken = NULL;
    }
    if (pRootMsg->m_threadGroupName != NULL)
    {
        sdsfree(pRootMsg->m_threadGroupName);
        pRootMsg->m_threadGroupName = NULL;
    }
    if (pRootMsg->m_threadId != NULL)
    {
        sdsfree(pRootMsg->m_threadId);
        pRootMsg->m_threadId = NULL;
    }
    if (pRootMsg->m_threadName != NULL)
    {
        sdsfree(pRootMsg->m_threadName);
        pRootMsg->m_threadName = NULL;
    }
}

void deleteCatRootMessage(CatRootMessage * pRootMsg)
{
    // delete 时需要把rootmessage一起删除
    if (pRootMsg->m_rootMsg != NULL)
    {
        deleteCatMessage(pRootMsg->m_rootMsg);
        pRootMsg->m_rootMsg = NULL;
    }

    clearCatRootMessage(pRootMsg);

    free(pRootMsg);
}

CatRootMessage * createCatRootMessage()
{
    return copyCatRootMessage(NULL);
}

