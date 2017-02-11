
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
CatRootMessage * getContextMessageTree();
int hasCatContext();
void resetCatContext();
int isTraceMode();
void setTraceMode(int mode);


void catContextAdd(CatMessage * message);
void catContextAddTransChild(CatMessage * message, CatTransaction * trans);
void catContextAdjustForTruncatedTrans(CatTransaction * root);
void catContextStartTrans(CatTransaction * trans);
int catContextEndTrans(CatTransaction * trans);
CatTransaction * catContextPeekTransaction();


#endif//CATCONTEXT_h__
