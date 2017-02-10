#ifndef CATTRACE_h__
#define CATTRACE_h__

#include "CatMessage.h"

typedef CatMessage CatTrace;


CatTrace * createCatTrace(const char *type, const char * name);


#endif//CATTRACE_h__
