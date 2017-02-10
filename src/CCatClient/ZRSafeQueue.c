#include "ZRSafeQueue.h"

ZRSafeQueue * createZRSafeQueue(size_t maxQueueSize)
{
    ZRSafeQueue * pQue = (ZRSafeQueue *)malloc(sizeof(ZRSafeQueue));
    if (pQue == NULL)
    {
        return NULL;
    }
    pQue->queue = createZRStaticQueue(maxQueueSize);
    if (pQue->queue == NULL)
    {
        free(pQue);
        return NULL;
    }
    pQue->lock = ZRCreateCriticalSection();
    SEMA_INIT(pQue->getSema, 0, maxQueueSize);
	return pQue;
}

int pushBackZRSafeQueue(ZRSafeQueue * pQueue, void * pData)
{
    ZRCS_ENTER(pQueue->lock);

    if (pushBackZRStaticQueue(pQueue->queue, pData) == ZRSTATICQUEUE_ERR)
    {
        ZRCS_LEAVE(pQueue->lock);
        return ZRSAFEQUEUE_ERR;
    }

    ZRCS_LEAVE(pQueue->lock);
    SEMA_POST(pQueue->getSema);
    return ZRSAFEQUEUE_OK;
}

int pushFrontZRSafeQueue(ZRSafeQueue * pQueue, void * pData)
{
    ZRCS_ENTER(pQueue->lock);

    if (pushFrontZRStaticQueue(pQueue->queue, pData) == ZRSTATICQUEUE_ERR)
    {
        ZRCS_LEAVE(pQueue->lock);
        return ZRSAFEQUEUE_ERR;
    }

    ZRCS_LEAVE(pQueue->lock);
    SEMA_POST(pQueue->getSema);
    return ZRSAFEQUEUE_OK;
}

void * popBackZRSafeQueue(ZRSafeQueue * pQueue, int maxWaitTime)
{
    if (SEMA_WAIT_TIME(pQueue->getSema, maxWaitTime) != SEMA_WAIT_OK)
    {
        return NULL;
    }

    ZRCS_ENTER(pQueue->lock);
    void * pData = popBackZRStaticQueue(pQueue->queue);
    ZRCS_LEAVE(pQueue->lock);

    return pData;
}

void * popFrontZRSafeQueue(ZRSafeQueue * pQueue, int maxWaitTime)
{
    if (SEMA_WAIT_TIME(pQueue->getSema, maxWaitTime) != SEMA_WAIT_OK)
    {
        return NULL;
    }

    ZRCS_ENTER(pQueue->lock);
    void * pData = popFrontZRStaticQueue(pQueue->queue);
    ZRCS_LEAVE(pQueue->lock);

    return pData;
}

int popBackManyZRSafeQueue(ZRSafeQueue * pQueue, void * pQueueArray[], int maxGetNum, int maxWaitTime)
{
    // 拿到很多意味着别人会空转
    int num = 0;
    if (SEMA_WAIT_TIME(pQueue->getSema, maxWaitTime) != SEMA_WAIT_OK)
    {
        return num;
    }

    ZRCS_ENTER(pQueue->lock);

    do
    {
        void * pData = popBackZRStaticQueue(pQueue->queue);
        if (pData != NULL)
        {
            pQueueArray[num++] = pData;
        }
        else
        {
            break;
        }
    } while (num < maxGetNum);

    ZRCS_LEAVE(pQueue->lock);

    return num;
}

int popFrontManyZRSafeQueue(ZRSafeQueue * pQueue, void * pQueueArray[], int maxGetNum, int maxWaitTime)
{


    // 拿到很多意味着别人会空转
    int num = 0;
    if (SEMA_WAIT_TIME(pQueue->getSema, maxWaitTime) != SEMA_WAIT_OK)
    {
        return num;
    }

    ZRCS_ENTER(pQueue->lock);

    do
    {
        void * pData = popFrontZRStaticQueue(pQueue->queue);
        if (pData != NULL)
        {
            pQueueArray[num++] = pData;
        }
        else
        {
            break;
        }
    } while (num < maxGetNum);

    ZRCS_LEAVE(pQueue->lock);

    return num;


}

void * pryBackZRSafeQueue(ZRSafeQueue * pQueue)
{
	
	ZRCS_ENTER(pQueue->lock);
	void * pData = pryBackZRStaticQueue(pQueue->queue);
	ZRCS_LEAVE(pQueue->lock);

	return pData;
}

void * pryFrontZRSafeQueue(ZRSafeQueue * pQueue)
{
	ZRCS_ENTER(pQueue->lock);
	void * pData = pryFrontZRStaticQueue(pQueue->queue);
	ZRCS_LEAVE(pQueue->lock);

	return pData;
}

void * getZRSafeQueueByIndex(ZRSafeQueue * pQueue, size_t index)
{
	ZRCS_ENTER(pQueue->lock);
	void * pData = getZRStaticQueueByIndex(pQueue->queue, index);
	ZRCS_LEAVE(pQueue->lock);

	return pData;
}

