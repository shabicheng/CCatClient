#include "CatTransaction.h"


#include "CatMessageManager.h"
#include "TimeUtility.h"
#include "CatEvent.h"


static void	addChild(CatTranscation* message, CatMessage* childMsg)
{

	CatTranscationInner * pInner = getInnerTrans(message);
	int pushRst = pushBackZRStaticQueue(pInner->m_children, childMsg);
	if (ZRSTATICQUEUE_ERR == pushRst)
	{
		// @todo 强制刷新再push
	}
}

static void * clear(CatMessage* message)
{
	CatTranscationInner * pInner = getInnerTrans(message);
	pInner->clear(message);
	for (size_t i = 0; i < getZRStaticQueueSize(pInner->m_children); ++i)
	{
		CatMessage * pMessage = getZRStaticQueueByIndex(pInner->m_children, i);
	}
	return pInner;
}

static void setTransactionComplete(CatMessage * message)
{
	CatTranscationInner * pInner = getInnerTrans(message);
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
		pInner->setCompleteFlag(message, 1);
		catMessageManagerEndTrans(message);
	}
}

CatTranscation * createCatTranscation(const char *type, const char * name)
{
	CatTranscationInner * pTransInner = malloc(sizeof(CatTranscation) + sizeof(CatTranscationInner));
	if (pTransInner == NULL)
	{
		return NULL;
	}
	CatTranscation * pTrans = (CatTranscation *)(((char *)pTransInner + sizeof(CatTranscation)));
	initCatMessage((CatMessage*)pTrans, CatMessageType_Trans, type, name);
	pTransInner->m_children = createZRStaticQueue(256);
	// 设置
	// 
	pTrans->setComplete = setTransactionComplete;
	pTrans->addChild = addChild;
	pTrans->clear = clear;
	return pTrans;
}

