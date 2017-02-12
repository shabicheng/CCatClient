#ifndef ZRSAFEQUEUE_H
#define ZRSAFEQUEUE_H

#include "ZRStaticQueue.h"
#include "ZRMultiThreadUtility.h"

#ifdef WIN32
#define inline __inline
#endif

// 固定长度，插入无阻塞型队列
typedef struct _ZRSafeQueue
{
    ZRStaticQueue * queue;
    ZRCRITICALSECTION lock;
    SEMA getSema;
}ZRSafeQueue;

#define ZRSAFEQUEUE_ERR -1
#define ZRSAFEQUEUE_OK 0

ZRSafeQueue * createZRSafeQueue(size_t maxQueueSize);

int pushBackZRSafeQueue(ZRSafeQueue * pQueue, void * pData);
int pushFrontZRSafeQueue(ZRSafeQueue * pQueue, void * pData);

void * popBackZRSafeQueue(ZRSafeQueue * pQueue, int maxWaitTime);
void * popFrontZRSafeQueue(ZRSafeQueue * pQueue, int maxWaitTime);

void * pryBackZRSafeQueue(ZRSafeQueue * pQueue);
void * pryFrontZRSafeQueue(ZRSafeQueue * pQueue);

void * getZRSafeQueueByIndex(ZRSafeQueue * pQueue, size_t index);

void clearZRSafeQueue(ZRSafeQueue * pQueue);

void destroyZRSafeQueue(ZRSafeQueue * pQueue);

// 下面这些加锁也没啥用，干脆不加
inline size_t getZRSafeQueueSize(ZRSafeQueue * pQueue)
{
	return getZRStaticQueueSize(pQueue->queue);
}
inline int isZRSafeQueueEmpty(ZRSafeQueue * pQueue)
{
	return isZRStaticQueueEmpty(pQueue->queue);
}
inline int isZRSafeQueueFull(ZRSafeQueue * pQueue)
{
	return isZRStaticQueueFull(pQueue->queue);
}

int popBackManyZRSafeQueue(ZRSafeQueue * pQueue, void * pQueueArray[], int maxGetNum, int maxWaitTime);
int popFrontManyZRSafeQueue(ZRSafeQueue * pQueue, void * pQueueArray[], int maxGetNum, int maxWaitTime);


typedef ZRSafeQueue ZRSafeStack;

#define createZRSafeStack createZRSafeQueue
#define pushZRSafeStack pushFrontZRSafeQueue
#define popZRSafeStack popFrontZRSafeQueue
#define pryZRSafeStack pryBackZRSafeQueue
#define getZRSafeStackSize getZRSafeQueueSize
#define isZRSafeStackEmpty isZRSafeQueueEmpty
#define isZRSafeStackFull isZRSafeQueueFull
#define getZRSafeStackByIndex getZRSafeQueueByIndex
#define clearZRSafeStack clearZRSafeQueue
#define destroyZRSafeStack destroyZRSafeQueue

typedef ZRSafeQueue ZRSafeFIFOQueue;

#define createZRSafeFIFOQueue createZRSafeQueue
#define inZRSafeFIFOQueue pushBackZRSafeQueue
#define outZRSafeFIFOQueue popFrontZRSafeQueue
#define pryZRSafeFIFOQueue pryFrontZRSafeQueue
#define getZRSafeFIFOQueueSize getZRSafeQueueSize
#define isZRSafeFIFOQueueEmpty isZRSafeQueueEmpty
#define isZRSafeFIFOQueueFull isZRSafeQueueFull
#define getZRSafeFIFOQueueByIndex getZRSafeQueueByIndex
#define clearZRSafeFIFOQueue clearZRSafeQueue
#define destroyZRSafeFIFOQueue destroyZRSafeQueue





#endif//ZRSAFEQUEUE_H