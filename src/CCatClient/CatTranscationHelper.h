
#ifndef CATTRANSCATIONHELPER_h__
#define CATTRANSCATIONHELPER_h__
#include "CatTransaction.h"
#include "CatContext.h"


int isCatTransaction(CatMessage* message);
int isCatEvent(CatMessage* message);
int isCatTrace(CatMessage* message);
int isCatMetric(CatMessage* message);
int isCatHeartBeat(CatMessage* message);

void truncateAndFlush(CatContext * context, unsigned long long timestampMs);

void validateTransaction(CatTransaction * pParentTrans, CatTransaction * pTrans);

#endif//CATTRANSCATIONHELPER_h__
