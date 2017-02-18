
#include "CatMessageMergeAndEncode.h"
#include "ZRMultiThreadUtility.h"
#include "ZRSafeQueue.h"
#include "CatClientConfig.h"
#include "CLog.h"
#include "TimeUtility.h"
#include "CatTransaction.h"
#include "CatEncoder.h"
#include "CatMessigeIdHelper.h"
#include "CatMessageSender.h"
#include "CatTranscationHelper.h"

static unsigned long long g_cat_msgQueFullCount = 0;


static ZRSafeQueue * g_cat_messageQueue = NULL;
static CatRootMessage * g_cat_mergeMessage = NULL;
static int g_cat_mergeCount = 0;
static volatile int g_cat_mergeStop = 0;

static sds g_cat_encodeBuf = NULL;

static void * g_cat_mergeAndEncodeHandle = NULL;


static int isAtomicMessage(CatRootMessage * pRootMsg)
{
    CatMessage * message = pRootMsg->m_rootMsg;

    if (isCatTransaction(message)) 
    {
        sds type = getCatMessageType(message);
        // start with "Cache." or equal "SQL"
        if ((sdslen(type) >= 6 && memcmp(type, "Cache.", 6) == 0) || sdscmp(type, "SQL") == 0) 
        {
            return 1;
        }
        else 
        {
            return 0;
        }
    }
    else 
    {
        return 1;
    }
}

void encodeAndSendBuffer(CatRootMessage * pRootMsg)
{
    sdsclear(g_cat_encodeBuf);
    g_cat_encodeBuf = catEncodeMessage(pRootMsg, g_cat_encodeBuf);
    sendCatMessageBuffer(g_cat_encodeBuf);

    deleteCatRootMessage(pRootMsg);
}


#ifdef _WIN32
static DWORD WINAPI catMessageMergeAndEncodeFun(PVOID para)
#else
static void* catMessageMergeAndEncodeFun(void* para)
#endif
{
    while (!g_cat_mergeStop)
    {
        CatRootMessage * pRootQueueMsg = popFrontZRSafeQueue(g_cat_messageQueue, 100);
        int mergeFlushFlag = 0;
        if (pRootQueueMsg != NULL)
        {


            // 判断是否需要merge
            if (isAtomicMessage(pRootQueueMsg)) 
            {
                CatTransaction * pTrans = NULL;
                CatMessage * pAtomicMsg = pRootQueueMsg->m_rootMsg;
                if (g_cat_mergeMessage->m_rootMsg == NULL)
                {
                    pTrans = createCatTransaction("C_CatMergeTree", "C_CatMergeTree");
                    catChecktPtr(pTrans);
                    g_cat_mergeMessage->m_rootMsg = (CatMessage *)pTrans;
                    if (g_cat_mergeMessage->m_messageId != NULL)
                    {
                        sdsfree(g_cat_mergeMessage->m_messageId);
                        g_cat_mergeMessage->m_messageId = NULL;
                    }
                    g_cat_mergeMessage->m_messageId = getNextMessageId();
                    pTrans->setStatus((CatMessage *)pTrans, CAT_SUCCESS);
                    // 这边的Complete只是设置complete标志位
                    CatMessageInner * msgInner = getInnerMsg(pTrans);
                    msgInner->setCompleteFlag((CatMessage *)pTrans, 1);
                    setCatMessageTimeStamp((CatMessage *)pTrans, getCatMessageTimeStamp(pAtomicMsg));
                }
                else
                {
                    pTrans = (CatTransaction *)g_cat_mergeMessage->m_rootMsg;
                }

                pTrans->addChild(pTrans, pRootQueueMsg->m_rootMsg);
                // 被拿走之后要置为NULL
                pRootQueueMsg->m_rootMsg = NULL;

                if (isCatTransaction(pAtomicMsg))
                {
                    setCatTranscationDurationUs(pTrans, getCatMessageTimeStamp(pAtomicMsg) * 1000 - 
                        getCatMessageTimeStamp((CatMessage *)pTrans) * 1000 +
                        getCatTranscationDurationUs((CatTransaction *)pAtomicMsg));
                }
                else
                {
                    setCatTranscationDurationUs(pTrans, getCatMessageTimeStamp(pAtomicMsg) * 1000 -
                        getCatMessageTimeStamp((CatMessage *)pTrans) * 1000);
                }

                reuseMessageId(pRootQueueMsg->m_messageId);
                // 回收之后也要置为NULL
                pRootQueueMsg->m_messageId = NULL;
                deleteCatRootMessage(pRootQueueMsg);



                if (getZRStaticStackSize(pTrans->getChildren(pTrans)) >= g_config.maxChildSize)
                {
                    mergeFlushFlag = 1;

                    encodeAndSendBuffer(g_cat_mergeMessage); 
                    // 删除之后要重新建立
                    g_cat_mergeMessage = createCatRootMessage();
                    catChecktPtr(g_cat_mergeMessage);
                }


            
            }
            else
            {
                encodeAndSendBuffer(pRootQueueMsg);
            }

        }

        if (!mergeFlushFlag && g_cat_mergeMessage->m_rootMsg != NULL)
        {
            // 检查是否需要强制发送，默认10秒强制刷新一次
            if (GetTime64() - getCatMessageTimeStamp(g_cat_mergeMessage->m_rootMsg) > 10 * 1000)
            {
                mergeFlushFlag = 1;

                encodeAndSendBuffer(g_cat_mergeMessage);
                // 删除之后要重新建立
                g_cat_mergeMessage = createCatRootMessage();
                catChecktPtr(g_cat_mergeMessage);
            }
        }
    }
    return 0;
}


int sendRootMessage(CatRootMessage * pRootMsg)
{
//     ++g_dbg_inCount;
//     if (g_dbg_inCount % 10000 == 0)
//     {
//         printf("sendRootMessage %lld %lld\n", g_dbg_inCount, g_dbg_outCount);
//     }
    if (pushBackZRSafeQueue(g_cat_messageQueue, pRootMsg) == ZRSAFEQUEUE_ERR)
    {
        if (g_cat_msgQueFullCount == 0 || g_cat_msgQueFullCount % 1000 == 0)
        {
            INNER_LOG(CLOG_WARNING, "当前root message队列已满.");
        }
        Sleep(1);
        ++g_cat_msgQueFullCount;
        return 0;
    }
    return 1;
}



void initCatMergeAndEncodeThread()
{
    g_cat_messageQueue = createZRSafeQueue(g_config.messageQueueSize);
    catChecktPtr(g_cat_messageQueue);
    g_cat_mergeMessage = createCatRootMessage();
    catChecktPtr(g_cat_mergeMessage);
    g_cat_mergeCount = 0;
    g_cat_mergeStop = 0;
    // 默认开4M的缓冲区
    g_cat_encodeBuf = sdsnewEmpty(4 * 1024 * 1024);
#ifdef WIN32
    g_cat_mergeAndEncodeHandle = _beginthreadex(NULL,
        0,
        catMessageMergeAndEncodeFun,
        0,
        0,
        NULL);
    SetThreadPriority(g_cat_mergeAndEncodeHandle, THREAD_PRIORITY_HIGHEST);
#else
    pthread_create(&g_cat_mergeAndEncodeHandle, NULL, catMessageMergeAndEncodeFun, NULL);
#endif
}

void clearCatMergeAndEncodeThread()
{
    // 等待线程退出
    g_cat_mergeStop = 1;
    // 删除线程

#ifdef _WIN32
    // 如果等待一秒还没有结束，则认为是有问题的，此时强制结束线程
    if (WAIT_OBJECT_0 != WaitForSingleObject(g_cat_mergeAndEncodeHandle, 1000))
    {
        TerminateThread(g_cat_mergeAndEncodeHandle, 0);
    }
    CloseHandle(g_cat_mergeAndEncodeHandle);
#else
    pthread_join(g_cat_mergeAndEncodeHandle, NULL);
#endif // _WIN32
    // 删除g_cat_messageQueue
    deleteCatRootMessage(g_cat_mergeMessage);
    // 删除g_cat_mergeMessage
	size_t i = 0;
    for (; i < getZRSafeQueueSize(g_cat_messageQueue); ++i)
    {
        deleteCatMessage((CatMessage *)getZRSafeQueueByIndex(g_cat_messageQueue, i));
    }
    destroyZRSafeQueue(g_cat_messageQueue);

    // free buffer
    sdsfree(g_cat_encodeBuf);
}
