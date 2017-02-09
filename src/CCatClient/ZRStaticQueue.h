#ifndef ZRSTATICQUEUE_H
#define ZRSTATICQUEUE_H
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef WIN32
#define inline __inline
#endif

typedef struct _ZRStaticQueue
{
    size_t maxQueueSize;
    size_t head;
    size_t tail;
    size_t size;
    void * valueArray[];
}ZRStaticQueue;

#define ZRSTATICQUEUE_ERR -1
#define ZRSTATICQUEUE_OK 0

ZRStaticQueue * createZRStaticQueue(size_t maxQueueSize);

int pushBackZRStaticQueue(ZRStaticQueue * pQueue, void * pData);
int pushFrontZRStaticQueue(ZRStaticQueue * pQueue, void * pData);

void * popBackZRStaticQueue(ZRStaticQueue * pQueue);
void * popFrontZRStaticQueue(ZRStaticQueue * pQueue);

void * pryBackZRStaticQueue(ZRStaticQueue * pQueue);
void * pryFrontZRStaticQueue(ZRStaticQueue * pQueue);

inline size_t getZRStaticQueueSize(ZRStaticQueue * pQueue)
{
    return pQueue->size;
}

inline int isZRStaticQueueEmpty(ZRStaticQueue * pQueue)
{
    return pQueue->size == 0;
}

inline int isZRStaticQueueFull(ZRStaticQueue * pQueue)
{
    return pQueue->size == pQueue->maxQueueSize;
}

inline int getZRStaticQueueRightDirect(ZRStaticQueue * pQueue)
{
    return pQueue->head - pQueue->tail;
}






#endif//ZRSTATICQUEUE_H

