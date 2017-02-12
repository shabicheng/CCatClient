#include "CatMessageSender.h"
#include "ZRMultiThreadUtility.h"
#include "ZRSafeQueue.h"
#include "CatClientConfig.h"
#include "CLog.h"
#include "TimeUtility.h"
#include "anet.h"
#include "CatServerConnManager.h"

static ZRSafeQueue * g_cat_bufferQueue = NULL;
static volatile int g_cat_senderStop = 0;


static void * g_cat_senderHandle = NULL;
static sds g_cat_mergeBuf = NULL;

volatile int g_cat_send_fd = -1;
volatile char g_cat_send_ip[64] = {0};
volatile unsigned short g_cat_send_port = 0;

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
    catChecktPtr(sendBuf);
    sds newBuf = sdsdup(sendBuf);
    catChecktPtr(newBuf);
    if (pushBackZRSafeQueue(g_cat_bufferQueue, newBuf) == ZRSAFEQUEUE_OK)
    {
        return 1;
    }
    sdsfree(newBuf);
    return 0;
}

int sendCatMessageBufferDirectly(sds sendBuf)
{
    // @debug
    //return 1;
    // µ÷ÓÃsocketÖ±½Ó·¢ËÍÁË
	
	
	//printf("SendBufLen %d\n", sdslen(sendBuf));
	//printf("SendBufLen %lld\n", sdslen(sendBuf));
	
    if (anetWrite(g_cat_send_fd, sendBuf, sdslen(sendBuf)) < 0)
    {
        INNER_LOG(CLOG_WARNING, "Ïò·þÎñÆ÷ip: %s ·¢ËÍÐÅÏ¢Ê§°Ü, ¿ªÊ¼³¢ÊÔ»Ö¸´Á¬½Ó.", g_cat_send_ip);
        recoverCatServerConn();
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
        // Õâ±ßÆäÊµ¿ÉÒÔºÏ°üÀ´·¢£¬ÊµÏÖ²ßÂÔÊÇ»ñÈ¡Ò»ÏµÁÐµÄ
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
                        // ÄÚ²¿Æ´½Ó
                        g_cat_mergeBuf = sdscat(g_cat_mergeBuf, sendBufArray[nowEle]);
                        sdsfree(sendBufArray[nowEle]);
                        ++nowEle;
                    }
                    // Æ´½ÓÍêÁË·¢ËÍ³öÈ¥£¬Õâ¸öbuf²»ÐèÒªÊÍ·Å
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
    // µÈ´ýÏß³ÌÍË³ö
    g_cat_senderStop = 1;
    // É¾³ýÏß³Ì

#ifdef _WIN32
    // Èç¹ûµÈ´ýÒ»Ãë»¹Ã»ÓÐ½áÊø£¬ÔòÈÏÎªÊÇÓÐÎÊÌâµÄ£¬´ËÊ±Ç¿ÖÆ½áÊøÏß³Ì
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
    // É¾³ýg_cat_bufferQueue
	size_t i = 0;
    for (; i < getZRSafeQueueSize(g_cat_bufferQueue); ++i)
    {
        sdsfree((sds)getZRSafeQueueByIndex(g_cat_bufferQueue, i));
    }
    destroyZRSafeQueue(g_cat_bufferQueue);

    sdsfree(g_cat_mergeBuf);
}
