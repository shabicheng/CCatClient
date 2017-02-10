
#ifndef CATCONTEXT_h__
#define CATCONTEXT_h__
#include "CatRootMessage.h"
#include "ZRStaticQueue.h"
#include "CatTransaction.h"

typedef struct _CatContext
{
	CatRootMessage * m_rootMsg;
	ZRStaticStack * m_transStack;
	int m_elementSize;
	int m_traceMode;
	unsigned long long m_lastTruncateTransDurationUs;

}CatContext;


CatContext * getCatContext();
CatRootMessage * getThreadLocalMessageTree();
int hasCatContext();
void resetCatContext();
int isTraceMode();
void setTraceMode(int mode);


void catContextAdd(CatMessage * message);
void catContextAddTransChild(CatMessage * message, CatTranscation * trans);
void catContextAdjustForTruncatedTrans(CatTranscation * root);
void catContextStartTrans(CatTranscation * trans);
void catContextEndTrans(CatTranscation * trans);
CatTranscation * catContextPeekTransaction();


#endif//CATCONTEXT_h__
