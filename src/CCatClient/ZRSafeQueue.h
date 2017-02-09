#ifndef ZRSAFEQUEUE_H
#define ZRSAFEQUEUE_H

#include "ZRStaticQueue.h"
#include "ZRMultiThreadUtility.h"

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

int popBackManyZRSafeQueue(ZRSafeQueue * pQueue, void * pQueueArray[], int maxGetNum, int maxWaitTime);
int popFrontManyZRSafeQueue(ZRSafeQueue * pQueue, void * pQueueArray[], int maxGetNum, int maxWaitTime);






#endif//ZRSAFEQUEUE_H