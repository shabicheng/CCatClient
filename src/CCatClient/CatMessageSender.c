#include "CatMessageSender.h"
#include "ZRMultiThreadUtility.h"
#include "ZRSafeQueue.h"
#include "CatClientConfig.h"
#include "CLog.h"
#include "TimeUtility.h"
#include "anet.h"
#include "CatServerConnManager.h"
#include "ae.h"

static ZRSafeQueue * g_cat_bufferQueue = NULL;
static volatile int g_cat_senderStop = 0;


static void * g_cat_senderHandle = NULL;
static sds g_cat_mergeBuf = NULL;

volatile int g_cat_send_fd = -1;
volatile char g_cat_send_ip[64] = {0};
volatile unsigned short g_cat_send_port = 0;
volatile unsigned long long g_cat_send_blockTimes = 0;
volatile int g_cat_send_failedFlag = 0;

#define  CAT_MERGEBUF_COUNT 16
#define  CAT_MERGEBUF_SIZE (60 * 1024)


// @debug

//FILE * g_cat_msgSaveFile = NULL;

// @debug end

int isCatSenderEnable()
{
//     // @debug
//     return 1;
//     // @debug end
    return g_cat_send_fd > 0;
}

int sendCatMessageBuffer(sds sendBuf)
{
    int sendTotalLen = sdslen(sendBuf);
    catChecktPtr(sendBuf);
    sds newBuf = sdsdup(sendBuf);
    catChecktPtr(newBuf);
    if (pushBackZRSafeQueue(g_cat_bufferQueue, newBuf) == ZRSAFEQUEUE_OK)
    {
        return 1;
    }
    ++g_cat_send_blockTimes;
    if (g_cat_send_blockTimes == 1 || g_cat_send_blockTimes % 1000 == 0)
    {
        INNER_LOG(CLOG_WARNING, "Server :  %s is blocking.", g_cat_send_ip);
    }
    sdsfree(newBuf);
    return 0;
}

int sendCatMessageBufferDirectly(sds sendBuf)
{
    // @debug
    //return 1;
    if (g_cat_send_failedFlag)
    {
        return -1;
    }
	
    if (g_cat_send_fd < 0)
    {
        INNER_LOG(CLOG_WARNING, "Ïò·þÎñÆ÷ip: %s ·¢ËÍÐÅÏ¢Ê§°Ü, ¿ªÊ¼³¢ÊÔ»Ö¸´Á¬½Ó.", g_cat_send_ip);
        recoverCatServerConn();
        if (g_cat_send_fd < 0)
        {
            return -1;
        }
    }
	

    static int s_count = 0;

    printf("SendBufLen %d %d\n", sdslen(sendBuf), ++s_count);
        
    int sendTotalLen = sdslen(sendBuf);
    char * buf = sendBuf;
    int nowSendLen = 0;
    int sendLen = 0;
    while (nowSendLen != sendTotalLen)
    {
        // to promote performance, do not call fun anetNoBlockWrite, use socket directly
        //sendLen = anetNoBlockWrite(g_cat_send_fd, sendBuf, sendLen);
#ifdef WIN32
        sendLen = send(g_cat_send_fd, buf, sendTotalLen - nowSendLen, 0);
#else
        sendLen = write(g_cat_send_fd, buf, sendTotalLen - nowSendLen);
#endif
        //if (nwritten == 0) return totlen;

        if (sendLen == -1)
        {

#ifdef WIN32
            if (WSAGetLastError() == WSAEWOULDBLOCK)
#else
            if (errno == EAGAIN)
#endif
            {
                sendLen = 0;
                ++g_cat_send_blockTimes;
                if (g_cat_send_blockTimes == 1 || g_cat_send_blockTimes % 10000 == 0)
                {
                    INNER_LOG(CLOG_WARNING, "Server :  %s is blocking.", g_cat_send_ip);
                }

                aeWait(g_cat_send_fd, AE_WRITABLE, 10);
            }
            else
            {
                INNER_LOG(CLOG_WARNING, "Send to server :  %s  failed.", g_cat_send_ip);
                nowSendLen = -1;
                break;
            }
        }
        nowSendLen += sendLen;
        buf += sendLen;
    }
    
    

    if (nowSendLen < 0)
    {
        INNER_LOG(CLOG_WARNING, "Ïò·þÎñÆ÷ip: %s ·¢ËÍÐÅÏ¢Ê§°Ü, ¿ªÊ¼³¢ÊÔ»Ö¸´Á¬½Ó.", g_cat_send_ip);
        recoverCatServerConn();

        if (g_cat_send_fd < 0)
        {
            INNER_LOG(CLOG_ERROR, "Recover failed.");
        }
    }
    else
    {

    }

    // @debug
    //if (g_cat_msgSaveFile == NULL)
    //{
    //    g_cat_msgSaveFile = fopen("buf.dat", "wb");
    //}

    //fwrite(sendBuf, sdslen(sendBuf), 1, g_cat_msgSaveFile);

   //fwrite("$$$$", 4, 1, g_cat_msgSaveFile);
   //fflush(g_cat_msgSaveFile);
    // @debug end
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
                sdsclear(g_cat_mergeBuf);
                int nowEle = 0;
                while (nowEle < eleNum)
                {
                    while (nowEle < eleNum && sdslen(g_cat_mergeBuf) < CAT_MERGEBUF_SIZE)
                    {
                        // 内部拼接
                        g_cat_mergeBuf = sdscat(g_cat_mergeBuf, sendBufArray[nowEle]);
                        sdsfree(sendBufArray[nowEle]);
                        ++nowEle;
                    }
                    // 拼接完了发送出去，这个buf不需要释放
                    sendCatMessageBufferDirectly(g_cat_mergeBuf);
                    sdsclear(g_cat_mergeBuf);
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
    g_cat_mergeBuf = sdsnewEmpty(2 * 1024 * 1024);
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

    pthread_join(g_cat_senderHandle, NULL);
#endif // _WIN32
    // 删除g_cat_bufferQueue
	size_t i = 0;
    for (; i < getZRSafeQueueSize(g_cat_bufferQueue); ++i)
    {
        sdsfree((sds)getZRSafeQueueByIndex(g_cat_bufferQueue, i));
    }
    destroyZRSafeQueue(g_cat_bufferQueue);

    sdsfree(g_cat_mergeBuf);
}
