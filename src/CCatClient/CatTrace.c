#include "CatTrace.h"

#include "CatMessageManager.h"


static void setTraceComplete(CatMessage * message)
{
	CatMessageInner * pInner = getInnerMsg(message);
	pInner->setCompleteFlag(message, 1);
	catMessageManagerAdd(message);
}


CatTrace * createCatTrace(const char *type, const char * name)
{
	CatMessageInner * pTraceInner = malloc(sizeof(CatTrace) + sizeof(CatMessageInner));
	if (pTraceInner == NULL)
	{
		return NULL;
	}
	CatTrace * pTrace = (CatTrace *)((char *)pTraceInner + sizeof(CatMessageInner));
	initCatMessage(pTrace, CatMessageType_Trace, type, name);

	// 设置
	// 
	pTrace->setComplete = setTraceComplete;
	return pTrace;
}



