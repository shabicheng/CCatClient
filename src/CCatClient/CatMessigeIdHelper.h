
#ifndef CATMESSIGEIDHELPER_h__
#define CATMESSIGEIDHELPER_h__

#include "sds.h"
#include <stdio.h>

void initMessageIdHelper();

void flushMessageIdMark();

sds getNextMessageId();

void reuseMessageId(sds msgId);



#endif//CATMESSIGEIDHELPER_h__
