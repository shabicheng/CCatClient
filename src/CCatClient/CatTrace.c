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
	CatTrace * pTrace = malloc(sizeof(CatTrace) + sizeof(CatMessageInner));
	if (pTrace == NULL)
	{
		return NULL;
	}
	initCatMessage(pTrace, CatMessageType_Trace, type, name);

	// ÉèÖÃ
	// 
	pTrace->setComplete = setTraceComplete;
	return pTrace;
}



