#include "CatTranscationHelper.h"
#include "CatContext.h"
#include "CatEvent.h"

void truncateAndFlush(CatContext * context, unsigned long long timestampMs)
{

}

void markAsNotCompleted(CatTranscation * pTrans)
{
	CatEvent * event = createCatEvent("cat", "BadInstrument");

	event->setStatus(event, "TransactionNotCompleted");
	event->setComplete(event);
	pTrans->addChild(pTrans, event);
	pTrans->setComplete((CatMessage *)pTrans);
}

void validateTransaction(CatTranscation * pParentTrans, CatTranscation * pTrans)
{
	CatTranscationInner * pTransInner = getInnerTrans(pTrans);
	ZRStaticQueue * pChildren = pTransInner->m_children;
	for (size_t i = 0; i < getZRStaticQueueSize(pChildren); ++i)
	{
		CatMessage * pMsg = getZRStaticQueueByIndex(pChildren, i);
		if (isCatTranscation(pMsg))
		{
			validateTransaction(pTrans, (CatTranscation*)pMsg);
		}
	}
	if (!isCatMessageComplete((CatMessage *)pTrans))
	{
		markAsNotCompleted(pTrans);
	}
}




int isCatTranscation(CatMessage* message)
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

