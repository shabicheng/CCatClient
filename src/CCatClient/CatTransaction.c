#include "CatTransaction.h"


#include "CatMessageManager.h"
#include "TimeUtility.h"
#include "CatEvent.h"
#include "CatClientConfig.h"
#include "CLog.h"


static void	addChild(CatTransaction* message, CatMessage* childMsg)
{

    CatTransactionInner * pInner = getInnerTrans(message);
    int pushRst = pushBackZRStaticQueue(pInner->m_children, childMsg);
    if (ZRSTATICQUEUE_ERR == pushRst)
    {
        // °´µÀÀí²»»á×ßµ½Õâ
        INNER_LOG(CLOG_ERROR, "ÎªTransÌí¼Ó×ÓÏûÏ¢Ê§°Ü£¬¶ÓÁÐÒÑÂú %d.", getZRStaticQueueSize(pInner->m_children));
    }
}

static void * clear(CatMessage* message)
{
    CatTransactionInner * pInner = getInnerTrans(message);
    pInner->clear(message);
	size_t i = 0;
    for (; i < getZRStaticQueueSize(pInner->m_children); ++i)
    {
        CatMessage * pMessage = getZRStaticQueueByIndex(pInner->m_children, i);
        deleteCatMessage(pMessage);
    }
    destroyZRStaticQueue(pInner->m_children);
    return pInner;
}

static void setTransactionComplete(CatTransaction * message)
{
    CatTransactionInner * pInner = getInnerTrans(message);
    // complete() was called more than once
    if (pInner->m_completeFlag)
    {
        CatEvent * evt = createCatEvent("cat", "BadInstrument");

        evt->setStatus(evt, "TransactionAlreadyCompleted");
        evt->setComplete(evt);
        addChild(message, evt);
    }
    else
    {
        pInner->m_durationUs = GetTime64() * 1000 - pInner->m_durationStart / 1000;
        pInner->setCompleteFlag((CatMessage *)message, 1);
        catMessageManagerEndTrans(message);
    }
}

static ZRStaticQueue * getChildren(CatTransaction* message)
{
    CatTransactionInner * pInner = getInnerTrans(message);
    return pInner->m_children;
}

CatTransaction * createCatTransaction(const char *type, const char * name)
{
    CatTransactionInner * pTransInner = malloc(sizeof(CatTransaction) + sizeof(CatTransactionInner));
    if (pTransInner == NULL)
    {
        return NULL;
    }
    CatTransaction * pTrans = (CatTransaction *)(((char *)pTransInner + sizeof(CatTransactionInner)));
    initCatMessage((CatMessage*)pTrans, CatMessageType_Trans, type, name);
    pTransInner->m_children = createZRStaticQueue(g_config.maxChildSize);
    pTransInner->m_durationStart = GetTime64() * 1000 * 1000;
    // ÉèÖÃ
    // 
    pTrans->setComplete = setTransactionComplete;
    pTrans->addChild = addChild;
    pTrans->clear = clear;
    pTrans->getChildren = getChildren;
    return pTrans;
}

unsigned long long getCatTranscationDurationUs(CatTransaction * trans)
{
    CatTransactionInner * pInner = getInnerTrans(trans);
    if (pInner->m_durationUs >= 0)
    {
        return pInner->m_durationUs;
    }
    else
    {
        unsigned long long tmpDuration = 0;
        size_t len = pInner->m_children == NULL ? 0 : getZRStaticStackSize(pInner->m_children);
        if (len > 0)
        {
            CatMessage * lastChild = getZRStaticStackByIndex(pInner->m_children, len - 1);
            CatMessageInner * lastChildInner = getInnerMsg(lastChild);
            tmpDuration = (lastChildInner->m_timeStampMs - pInner->m_timeStamp) * 1000;
//             if (isCatTransaction(lastChild))
//             {
//                 CatTransactionInner * pInner = getInnerTrans(trans);
//                 DefaultTransaction trx = (DefaultTransaction)lastChild;
// 
//                 duration = (trx.getTimestamp() - getTimestamp()) * 1000L;
//             }
//             else 
//             {
//                 duration = (lastChild.getTimestamp() - getTimestamp()) * 1000L;
//             }
        }
        return tmpDuration;
    }
    
}

CatTransaction * copyCatTransaction(CatTransaction * pSrcTrans)
{
    CatTransactionInner * pSrcTransInner = getInnerTrans(pSrcTrans);
    CatTransaction * clonedTrans = createCatTransaction(pSrcTransInner->m_type, pSrcTransInner->m_name);
    CatTransactionInner * clonedTransInner = getInnerTrans(clonedTrans);
    clonedTransInner->m_timeStamp = pSrcTransInner->m_timeStamp;
    clonedTransInner->m_durationUs = getCatTranscationDurationUs(pSrcTrans);
    clonedTransInner->m_data = sdsdup(pSrcTransInner->m_data);
    return clonedTrans;
}


