
#ifndef CATENCODER_h__
#define CATENCODER_h__
#include "CatRootMessage.h"
#include "sds.h"

sds catEncodeMessage(CatRootMessage * pRootMsg, sds buf);

#endif//CATENCODER_h__
