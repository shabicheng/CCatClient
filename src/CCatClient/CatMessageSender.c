#include "CatMessageSender.h"
#include "ZRMultiThreadUtility.h"
#include "ZRSafeQueue.h"
#include "CatClientConfig.h"
#include "CLog.h"
#include "TimeUtility.h"
#include "anet.h"

static ZRSafeQueue * g_cat_bufferQueue = NULL;
static volatile int g_cat_senderStop = 0;


static void * g_cat_senderHandle = NULL;
static sds g_cat_mergeBuf = NULL;

volatile int g_cat_send_fd = -1;
volatile char g_cat_send_ip[64] = {0};
volatile unsigned short g_cat_send_port = 0;

#define  CAT_MERGEBUF_COUNT 16
#define  CAT_MERGEBUF_SIZE (60 * 1024)

int isCatSenderEnable()
{
    return g_cat_send_fd > 0;
}

int sendCatMessageBuffer(sds sendBuf)
{
    sds newBuf = sdsdup(sendBuf);
    catChecktPtr(sendBuf);
    if (pushBackZRSafeQueue(g_cat_bufferQueue, sendBuf) == ZRSAFEQUEUE_OK)
    {
        return 1;
    }
    sdsfree(newBuf);
    return 0;
}

int sendCatMessageBufferDirectly(sds sendBuf)
{
    // 调用socket直接发送了
    if (anetWrite(g_cat_send_fd, sendBuf, sdslen(sendBuf)) < 0)
    {
        INNER_LOG(CLOG_WARNING, "向服务器ip: %s 发送信息失败, 开始尝试恢复连接.", g_cat_send_ip);
    }
    return 1;
}


#ifdef _WIN32
static DWORD WINAPI catMessageSenderFun(PVOID para)
#else
static void* catMessageSenderFun(void* para)
#endif
{
    sds sendBufArray[CAT_MERGEBUF_COUNT];
    while (!g_cat_senderStop)
    {
        int eleNum = popFrontManyZRSafeQueue(g_cat_bufferQueue, sendBufArray, 16, 100);
        // 这边其实可以合包来发，实现策略是获取一系列的
        if (eleNum > 0)
        {
            if (eleNum > 1)
            {
                g_cat_mergeBuf = sdscpylen(g_cat_mergeBuf, sendBufArray[0], sdslen(sendBufArray[0]));
                int nowEle = 1;
                while (nowEle < eleNum)
                {
                    while (nowEle < eleNum && sdslen(g_cat_mergeBuf) < CAT_MERGEBUF_SIZE)
                    {
                        // 内部拼接
                        g_cat_mergeBuf = sdscat(g_cat_mergeBuf, sendBufArray[nowEle++]);
                        sdsfree(sendBufArray[nowEle++]);
                    }
                    // 拼接完了发送出去，这个buf不需要释放
                    sendCatMessageBufferDirectly(g_cat_mergeBuf);
                }

            }
            else
            {
                sendCatMessageBufferDirectly(sendBufArray[0]);
                sdsfree(sendBufArray[0]);

            }
        }
    }
    return 0;
}

void initCatSenderThread()
{
    g_cat_mergeBuf = sdsnewlen(NULL, 2 * 1024 * 1024);
    g_cat_bufferQueue = createZRSafeQueue(g_config.messageQueueSize);
    catChecktPtr(g_cat_bufferQueue);
    g_cat_senderStop = 0;
#ifdef WIN32
    g_cat_senderHandle = _beginthreadex(NULL,
        0,
        catMessageSenderFun,
        0,
        0,
        NULL);
    SetThreadPriority(g_cat_senderHandle, THREAD_PRIORITY_HIGHEST);
#else
    pthread_create(&g_cat_senderHandle, NULL, catMessageSenderFun, NULL);
#endif
}

void clearCatSenderThread()
{
    // 等待线程退出
    g_cat_senderStop = 1;
    // 删除线程

#ifdef _WIN32
    // 如果等待一秒还没有结束，则认为是有问题的，此时强制结束线程
    if (WAIT_OBJECT_0 != WaitForSingleObject(g_cat_senderHandle, 1000))
    {
        TerminateThread(g_cat_senderHandle, 0);
    }
    CloseHandle(g_cat_senderHandle);
#else
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
    {
        /* Handle error */
    }

    ts.tv_sec += 1;
    if (pthread_timedjoin_np(g_cat_senderHandle, NULL, &ts) != 0)
    {
        pthread_cancel(g_cat_senderHandle);
    }
#endif // _WIN32
    // 删除g_cat_bufferQueue
    for (size_t i = 0; i < getZRSafeQueueSize(g_cat_bufferQueue); ++i)
    {
        sdsfree((sds)getZRSafeQueueByIndex(g_cat_bufferQueue, i));
    }
    destroyZRSafeQueue(g_cat_bufferQueue);

    sdsfree(g_cat_mergeBuf);
}
