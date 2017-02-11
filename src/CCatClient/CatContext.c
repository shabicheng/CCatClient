#include "CatContext.h"
#include "CatClientCommon.h"
#include "CatRootMessage.h"
#include "CatClientConfig.h"
#include "CatTranscationHelper.h"
#include "CatEvent.h"
#include "CatMessageManager.h"

extern CatMessageManager g_cat_messageManager;

CATTHREADLOCAL CatContext * g_cat_context = NULL;

CatContext * getCatContext()
{
    if (g_cat_context == NULL)
    {
        g_cat_context = (CatContext *)malloc(sizeof(CatContext));
        g_cat_context->m_elementSize = 0;
        g_cat_context->m_rootMsg = createCatRootMessage();

        g_cat_context->m_rootMsg->m_rootMsg = NULL;

        // @todo 
        g_cat_context->m_rootMsg->m_threadId = sdsnew("0");
        g_cat_context->m_rootMsg->m_threadGroupName = sdsnew("UnknownGroup");
        g_cat_context->m_rootMsg->m_threadName = sdsnew("UnknownThread");


    }
    return g_cat_context;
}

CatRootMessage * getContextMessageTree()
{
    return g_cat_context->m_rootMsg;
}

int hasCatContext()
{
    return g_cat_context != NULL;
}

void resetCatContext()
{
    g_cat_context->m_elementSize = 0;
    g_cat_context->m_rootMsg = NULL;
    g_cat_context->m_lastTruncateTransDurationUs = 0;
    clearZRStaticStack(g_cat_context->m_transStack);
}

int isTraceMode()
{
    return g_cat_context->m_traceMode;
}

void setTraceMode(int mode)
{
    g_cat_context->m_traceMode = mode;
}

void catContextAdd(CatMessage * message)
{
    getCatContext();
    ZRStaticStack * pStack = g_cat_context->m_transStack;
    if (isZRStaticStackEmpty(pStack)) 
    {
        CatRootMessage * pRootCopy = copyCatRootMessage(g_cat_context->m_rootMsg);
        pRootCopy->m_rootMsg = message;
        catMessageManagerFlush(pRootCopy);
    }
    else 
    {
        CatTransaction * parent = pryZRStaticStack(pStack);

        catContextAddTransChild(message, parent);
    }
}

void catContextAddTransChild(CatMessage * message, CatTransaction * trans)
{
    unsigned long long treePeriod = catTrimToHour(getCatMessageTimeStamp(g_cat_context->m_rootMsg->m_rootMsg));
    unsigned long long messagePeriod = catTrimToHour(getCatMessageTimeStamp(message) - 10 * 1000L); // 10 seconds extra time allowed

    if (treePeriod < messagePeriod || g_cat_context->m_elementSize >= g_config.maxContextElementSize) 
    {
        truncateAndFlush(g_cat_context, getCatMessageTimeStamp(message));
    }

    trans->addChild(trans, message);
    ++g_cat_context->m_elementSize;
}

void catContextAdjustForTruncatedTrans(CatTransaction * root)
{
    CatEvent * next = createCatEvent("TruncatedTransaction", "TotalDuration");
    unsigned long long actualDurationUs = g_cat_context->m_lastTruncateTransDurationUs + getCatTranscationDurationUs(root);

    next->addDataPair(next, sdsfromlonglong(actualDurationUs));
    next->setStatus(next, CAT_SUCCESS);
    root->addChild(root, next);

    g_cat_context->m_lastTruncateTransDurationUs = 0;
}

void catContextStartTrans(CatTransaction * trans)
{
    getCatContext();
    if (!isZRStaticStackEmpty(g_cat_context->m_transStack)) 
    {
        
        CatTransaction * parent = pryZRStaticStack(g_cat_context->m_transStack);
        catContextAddTransChild((CatMessage *)trans, parent);
    }
    else 
    {
        g_cat_context->m_rootMsg->m_rootMsg = (CatMessage *)trans;
    }
    
    
    pushZRStaticStack(g_cat_context->m_transStack, trans);
    
}

int catContextEndTrans(CatTransaction * trans)
{
    size_t i, j;
    size_t initStackSize = getZRStaticStackSize(g_cat_context->m_transStack);
    // 只需要查找到该trans就可以
    // 如果这个trans不属于这个线程的局部区，那会出问题，所以可以先在不破坏栈的情况下遍历找到，然后再pop
    for (i = 0; i < initStackSize; ++i)
    {
        CatTransaction * stackTrans = getZRStaticStackByIndex(g_cat_context->m_transStack, i);
        if (stackTrans == trans)
        {
            break;
        }
    }
    if (i != initStackSize)
    {
        // 如果找到再真正的pop
        for (j = 0; j < i; ++i)
        {
            popZRStaticStack(g_cat_context->m_transStack);
        }
        validateTransaction(NULL, trans);
        if (isZRStaticStackEmpty(g_cat_context->m_transStack)) 
        {
            CatRootMessage * pCopyRoot = copyCatRootMessage(g_cat_context->m_rootMsg);

            // @todo rootMsg 里面字符串的内存管理需要完了之后整体考虑
            sdsfree(g_cat_context->m_rootMsg->m_messageId);
            g_cat_context->m_rootMsg->m_messageId = NULL;
            g_cat_context->m_rootMsg->m_rootMsg = NULL;

            if (g_cat_context->m_lastTruncateTransDurationUs> 0) 
            {
                catContextAdjustForTruncatedTrans((CatTransaction *)pCopyRoot->m_rootMsg);
            }

            catMessageManagerFlush(pCopyRoot);
            return 1;
        }
    }
    return 0;
}

CatTransaction * catContextPeekTransaction()
{
    if (!isZRStaticStackEmpty(g_cat_context->m_transStack))
    {
        return pryZRStaticStack(g_cat_context->m_transStack);
    }
    return NULL;
}

