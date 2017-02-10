#ifndef CATHEARTBEAT_h__
#define CATHEARTBEAT_h__



#include "CatMessage.h"

typedef CatMessage CatHeartBeat;


CatHeartBeat * createCatHeartBeat(const char *type, const char * name);

#endif//CATHEARTBEAT_h__
