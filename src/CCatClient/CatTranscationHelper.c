#include "CatTranscationHelper.h"
#include "CatContext.h"
#include "CatEvent.h"
#include "TimeUtility.h"


void migrateMessage(ZRStaticStack * pStack, CatTransaction * source, CatTransaction * target, int level)
{
    // 拿到下一级的trans
    CatTransaction * current = level < getZRStaticStackSize(pStack) ? (CatTransaction *)getZRStaticStackSize(pStack, level) : NULL;
    int shouldKeep = 0;
    CatTransactionInner * currentInner = getInnerTrans(current);

    ZRStaticQueue * children = source->getChildren(source);
    size_t i = 0;
    for (i = 0; i < getZRStaticQueueSize(children); ++i)
    {
        CatMessage * pMsg = getZRStaticQueueByIndex(children, i);
        if (pMsg != current)
        {
            target->addChild(target, pMsg);
        }
        else
        {


            CatTransaction * clonedTrans = copyCatTransaction(current);
            clonedTrans->setStatus(clonedTrans, CAT_SUCCESS);


            target->addChild(target, clonedTrans);
            migrateMessage(pStack, current, clonedTrans, level + 1);
            shouldKeep = 1;
        }
    }


    clearZRStaticQueue(children);

    if (shouldKeep) { // add it back
        pushBackZRSafeQueue(children, current);
    }

  
}

void truncateAndFlush(CatContext * context, unsigned long long timestampMs)
{
    CatRootMessage * pRootMsg = context->m_rootMsg;
    ZRStaticStack * pStack = context->m_transStack;
    CatMessage * message = pRootMsg->m_message;

    if (!isCatTransaction(message))
    {
        return;
    }
    // 注意，这个id是从pRootMsg拿出来的，及时为NULL，最终也会初始化并设置到pRootMsg上
    sds id = pRootMsg->m_messageId;

    if (id == NULL) {
        id = getNextMessageId();
        pRootMsg->m_messageId = id;
    }
    // 注意，这个id是从pRootMsg拿出来的
    sds rootId = pRootMsg->m_rootMessageId;
    // 注意，这个id是生成出来的
    sds childId = getNextMessageId();

    CatTransaction * source = (CatTransaction *)message;
    CatTransactionInner * sourceInner = getInnerTrans(source);


    CatTransaction * target = copyCatTransaction(source);
    target->setStatus(target, CAT_SUCCESS);


    migrateMessage(pStack, source, target, 1);

    for (size_t i = getZRStaticQueueSize(pStack) - 1; i >= 0; --i) 
    {
        CatTransaction * t = (CatTransaction *)getZRStaticQueueByIndex(pStack, i);
        CatTransactionInner * iInner = getInnerTrans(t);
        iInner->m_timeStamp = timestampMs;
        iInner->m_durationStart = GetTime64() * 1000 * 1000;
    }

    CatEvent * next = createCatEvent("RemoteCall", "Next");
    


    next->addDataPair(next, childId);
    next->setStatus(next, CAT_SUCCESS);
    target->addChild(target, next);

    // tree is the parent, and m_tree is the child.
    CatRootMessage * pCp = copyCatRootMessage(pRootMsg);

    pCp->m_message = target;

    // 注意，childId 是new出来的 id rootId 本身就是pRootMsg，所以不需要再次sdsdup出来
    pRootMsg->m_messageId = childId;
    pRootMsg->m_parentMessageId = id;
    pRootMsg->m_rootMessageId = (rootId != NULL ? rootId : sdsdup(id));

    context->m_elementSize = getZRStaticStackSize(pStack);
    context->m_lastTruncateTransDurationUs = context->m_lastTruncateTransDurationUs + getCatTranscationDurationUs(target);

    catMessageManagerFlush(pCp);
}

void markAsNotCompleted(CatTransaction * pTrans)
{
	CatEvent * event = createCatEvent("cat", "BadInstrument");

	event->setStatus(event, "TransactionNotCompleted");
	event->setComplete(event);
	pTrans->addChild(pTrans, event);
	pTrans->setComplete((CatMessage *)pTrans);
}

void validateTransaction(CatTransaction * pParentTrans, CatTransaction * pTrans)
{
	CatTransactionInner * pTransInner = getInnerTrans(pTrans);
	ZRStaticQueue * pChildren = pTransInner->m_children;
	for (size_t i = 0; i < getZRStaticQueueSize(pChildren); ++i)
	{
		CatMessage * pMsg = getZRStaticQueueByIndex(pChildren, i);
		if (isCatTransaction(pMsg))
		{
			validateTransaction(pTrans, (CatTransaction*)pMsg);
		}
	}
	if (!isCatMessageComplete((CatMessage *)pTrans))
	{
		markAsNotCompleted(pTrans);
	}
}




int isCatTransaction(CatMessage* message)
{
	CatMessageInner * pInner = getInnerMsg(message);
	return pInner->m_msgType == CatMessageType_Trans;
}

int isCatEvent(CatMessage* message)
{

	CatMessageInner * pInner = getInnerMsg(message);
	return pInner->m_msgType == CatMessageType_Event;
}

int isCatTrace(CatMessage* message)
{

	CatMessageInner * pInner = getInnerMsg(message);
	return pInner->m_msgType == CatMessageType_Trace;
}

int isCatMetric(CatMessage* message)
{

	CatMessageInner * pInner = getInnerMsg(message);
	return pInner->m_msgType == CatMessageType_Metric;
}

int isCatHeartBeat(CatMessage* message)
{

	CatMessageInner * pInner = getInnerMsg(message);
	return pInner->m_msgType == CatMessageType_HeartBeat;
}

