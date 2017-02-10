#include "CatEvent.h"
#include "CatMessageManager.h"


static void setEventComplete(CatMessage * message)
{
	CatMessageInner * pInner = getInnerMsg(message);
	pInner->setCompleteFlag(message, 1);
	catMessageManagerAdd(message);
}


CatEvent * createCatEvent(const char *type, const char * name)
{
	CatMessageInner * pEventInner = malloc(sizeof(CatEvent) + sizeof(CatMessageInner));
	if (pEventInner == NULL)
	{
		return NULL;
	}
	CatEvent * pEvent = (CatEvent *)((char *)pEventInner + sizeof(CatMessageInner));
	initCatMessage(pEvent, CatMessageType_Event, type, name);

	// ÉèÖÃ
	// 
	pEvent->setComplete = setEventComplete;
	return pEvent;
}

