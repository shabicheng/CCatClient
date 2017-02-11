
#ifndef CATMESSAGESENDER_h__
#define CATMESSAGESENDER_h__
#include "CatRootMessage.h"


void initCatSenderThread();
void clearCatSenderThread();
int isCatSenderEnable();
int sendCatMessageBuffer(sds sendBuf);

#endif//CATMESSAGESENDER_h__
