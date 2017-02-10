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
	volatile size_t head;
	volatile size_t tail;
    volatile size_t size;
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

void * getZRStaticQueueByIndex(ZRStaticQueue * pQueue, size_t index);

void clearZRStaticQueue(ZRStaticQueue * pQueue);
void destroyZRStaticQueue(ZRStaticQueue * pQueue);

inline size_t getZRStaticQueueSize(ZRStaticQueue * pQueue)
{
    return pQueue->size;
}

inline int isZRStaticQueueEmpty(ZRStaticQueue * pQueue)
{
    return pQueue->size;
}

inline int isZRStaticQueueFull(ZRStaticQueue * pQueue)
{
	return !(pQueue->maxQueueSize - pQueue->size);
}

inline int getZRStaticQueueRightDirect(ZRStaticQueue * pQueue)
{
    return pQueue->head - pQueue->tail;
}

typedef ZRStaticQueue ZRStaticStack;

#define pushZRStaticStack pushFrontZRStaticQueue
#define popZRStaticStack popFrontZRStaticQueue
#define pryZRStaticStack pryFrontZRStaticQueue
#define getZRStaticStackSize getZRStaticQueueSize
#define isZRStaticStackEmpty isZRStaticQueueEmpty
#define isZRStaticStackFull isZRStaticQueueFull
#define getZRStaticStackByIndex getZRStaticQueueByIndex
#define clearZRStaticStack clearZRStaticQueue
#define destroyZRStaticStack destroyZRStaticQueue

typedef ZRStaticQueue ZRStaticFIFOQueue;

#define inZRStaticFIFOQueue pushBackZRStaticQueue
#define outZRStaticFIFOQueue popFrontZRStaticQueue
#define pryZRStaticFIFOQueue pryFrontZRStaticQueue
#define getZRStaticFIFOQueueSize getZRStaticQueueSize
#define isZRStaticFIFOQueueEmpty isZRStaticQueueEmpty
#define isZRStaticFIFOQueueFull isZRStaticQueueFull
#define getZRStaticFIFOQueueByIndex getZRStaticQueueByIndex
#define clearZRStaticFIFOQueue clearZRStaticQueue
#define destroyZRStaticFIFOQueue destroyZRStaticQueue





#endif//ZRSTATICQUEUE_H

