
#include "ZRStaticQueue.h"

ZRStaticQueue * createZRStaticQueue(size_t maxQueueSize)
{
    ZRStaticQueue * pQue = (ZRStaticQueue *)malloc(sizeof(ZRStaticQueue) + maxQueueSize * sizeof(void *));
    if (pQue == NULL)
    {
        return NULL;
    }
    pQue->head = 0;
    pQue->tail = 0;
    pQue->size = 0;
    pQue->maxQueueSize = maxQueueSize;
    return pQue;
}

int pushBackZRStaticQueue(ZRStaticQueue * pQueue, void * pData)
{
    if (isZRStaticQueueFull(pQueue))
    {
        return ZRSTATICQUEUE_ERR;
    }

    // 0特殊处理，头尾相等
    if (isZRStaticQueueEmpty(pQueue))
    {
        pQueue->tail = 0;
        pQueue->head = 0;
        pQueue->size = 1;
        pQueue->valueArray[0] = pData;
        return ZRSTATICQUEUE_OK;
    }

    if (++pQueue->head == pQueue->maxQueueSize)
    {
        pQueue->head = 0;
    }
    pQueue->valueArray[pQueue->head] = pData;

    return ZRSTATICQUEUE_OK;
}

int pushFrontZRStaticQueue(ZRStaticQueue * pQueue, void * pData)
{
    if (isZRStaticQueueFull(pQueue))
    {
        return ZRSTATICQUEUE_ERR;
    }

    // 0特殊处理，头尾相等
    if (isZRStaticQueueEmpty(pQueue))
    {
        pQueue->tail = 0;
        pQueue->head = 0;
        pQueue->size = 1;
        pQueue->valueArray[0] = pData;
        return ZRSTATICQUEUE_OK;
    }

    if (--pQueue->tail < 0)
    {
        pQueue->tail = pQueue->maxQueueSize - 1;
    }
    pQueue->valueArray[pQueue->tail] = pData;

    return ZRSTATICQUEUE_OK;
}

void * popBackZRStaticQueue(ZRStaticQueue * pQueue)
{
    if (isZRStaticQueueEmpty(pQueue))
    {
        return NULL;
    }

    void * pData = pQueue->valueArray[pQueue->head];

    if (--pQueue->size == 0)
    {
        pQueue->tail = 0;
        pQueue->head = 0;
        pQueue->size = 0;
    }
    else
    {
        if (--pQueue->head < 0)
        {
            pQueue->head = pQueue->maxQueueSize - 1;
        }
    }

    return pData;
}

void * popFrontZRStaticQueue(ZRStaticQueue * pQueue)
{
    if (isZRStaticQueueEmpty(pQueue))
    {
        return NULL;
    }
    void * pData = pQueue->valueArray[pQueue->tail];

    if (--pQueue->size == 0)
    {
        pQueue->tail = 0;
        pQueue->head = 0;
        pQueue->size = 0;
    }
    else
    {
        if (++pQueue->tail == pQueue->maxQueueSize)
        {
            pQueue->tail = 0;
        }
    }

    return pData;
}

void * pryBackZRStaticQueue(ZRStaticQueue * pQueue)
{
    if (isZRStaticQueueEmpty(pQueue))
    {
        return NULL;
    }

    return pQueue->valueArray[pQueue->head];
}

void * pryFrontZRStaticQueue(ZRStaticQueue * pQueue)
{
    if (isZRStaticQueueEmpty(pQueue))
    {
        return NULL;
    }

    return pQueue->valueArray[pQueue->tail];
}

void * getZRStaticQueueByIndex(ZRStaticQueue * pQueue, size_t index)
{
	// unsafe
	if (!pQueue->size || pQueue->size - 1 < index)
	{
		return NULL;
	}
	index += pQueue->tail;
	if (index > pQueue->maxQueueSize)
	{
		index -= pQueue->maxQueueSize;
	}
	return pQueue->valueArray[index];
}

void clearZRStaticQueue(ZRStaticQueue * pQueue)
{
    pQueue->head = 0;
    pQueue->size = 0;
    pQueue->tail = 0;
}

void destroyZRStaticQueue(ZRStaticQueue * pQueue)
{
    free(pQueue);
}
