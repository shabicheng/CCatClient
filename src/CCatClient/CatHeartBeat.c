#include "CatHeartBeat.h"

#include "CatMessageManager.h"


static void setHeartBeatComplete(CatMessage * message)
{
	CatMessageInner * pInner = getInnerMsg(message);
	pInner->setCompleteFlag(message, 1);
	catMessageManagerAdd(message);
}


CatHeartBeat * createCatHeartBeat(const char *type, const char * name)
{
	CatMessageInner * pHBInner = malloc(sizeof(CatHeartBeat) + sizeof(CatMessageInner));
	if (pHBInner == NULL)
	{
		return NULL;
	}
	CatHeartBeat * pHB = (CatHeartBeat *)((char *)pHBInner + sizeof(CatMessageInner));


	initCatMessage(pHB, CatMessageType_HeartBeat, type, name);

	// ÉèÖÃ
	// 
	pHB->setComplete = setHeartBeatComplete;
	return pHB;
}

