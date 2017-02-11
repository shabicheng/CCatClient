
#ifndef CATMESSAGEMERGEANDENCODE_h__
#define CATMESSAGEMERGEANDENCODE_h__
#include "CatRootMessage.h"

void initCatMergeAndEncodeThread();
void clearCatMergeAndEncodeThread();
int sendRootMessage(CatRootMessage * pRootMsg);


#endif//CATMESSAGEMERGEANDENCODE_h__
