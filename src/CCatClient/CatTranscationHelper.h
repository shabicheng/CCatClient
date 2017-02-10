
#ifndef CATTRANSCATIONHELPER_h__
#define CATTRANSCATIONHELPER_h__
#include "CatTransaction.h"
#include "CatContext.h"


int isCatTranscation(CatMessage* message);
int isCatEvent(CatMessage* message);
int isCatTrace(CatMessage* message);
int isCatMetric(CatMessage* message);
int isCatHeartBeat(CatMessage* message);

void truncateAndFlush(CatContext * context, unsigned long long timestampMs);

void validateTransaction(CatTranscation * pParentTrans, CatTranscation * pTrans);

#endif//CATTRANSCATIONHELPER_h__
