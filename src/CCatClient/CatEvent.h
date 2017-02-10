
#ifndef CATEVENT_h__
#define CATEVENT_h__

#include "CatMessage.h"

typedef CatMessage CatEvent;


CatEvent * createCatEvent(const char *type, const char * name);


#endif//CATEVENT_h__
