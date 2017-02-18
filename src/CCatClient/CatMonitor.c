#include "CatMonitor.h"
#include "ZRMultiThreadUtility.h"
#include "ZRSafeQueue.h"
#include "CatClientConfig.h"
#include "CLog.h"
#include "TimeUtility.h"
#include "CatHeartBeat.h"
#include "CatServerConnManager.h"
#include "CatClient.h"

#include "CatMessageManager.h"

extern CatMessageManager g_cat_messageManager;

static volatile int g_cat_monitorStop = 0;

static void * g_cat_monitorHandle = NULL;

static CatHeartBeat * g_cat_heartbeat = NULL;

#ifdef _WIN32
static DWORD WINAPI catMonitorFun(PVOID para)
#else
static void* catMonitorFun(void* para)
#endif
{
    Sleep(1000);
    CatTransaction * reboot = newTransaction("System", "Reboot");

    reboot->setStatus((CatMessage *)reboot, CAT_SUCCESS);
    logEvent("Reboot", g_cat_messageManager.m_ip, CAT_SUCCESS, NULL);
    reboot->setComplete((CatMessage *)reboot);



    unsigned long runCount = 0;
    while (!g_cat_monitorStop)
    {
        // 每十秒更新连接
        if (++runCount % 10 == 0)
        {
            updateCatServerConn();
        }

        if (runCount % 10 == 0)
        {
            CatTransaction * t = newTransaction("System", "Status");
            CatHeartBeat * h = newHeartBeat("Heartbeat", g_cat_messageManager.m_ip);
            h->addDataPair(h, "CPU=3.3GHz");
            h->setStatus(h, CAT_SUCCESS);
            h->setComplete((CatMessage *)h);
            t->setStatus((CatMessage *)t, CAT_SUCCESS);
            t->setComplete((CatMessage *)t);
        }

        Sleep(1000);
    }
    return 0;
}

void initCatMonitor()
{
    g_cat_monitorStop = 0;

#ifdef WIN32
    g_cat_monitorHandle = _beginthreadex(NULL,
        0,
        catMonitorFun,
        0,
        0,
        NULL);
    //SetThreadPriority(g_cat_monitorHandle, THREAD_PRIORITY_HIGHEST);
#else
    pthread_create(&g_cat_monitorHandle, NULL, catMonitorFun, NULL);
#endif
}

void clearCatMonitor()
{
    // 等待线程退出
    g_cat_monitorStop = 1;
    // 删除线程

#ifdef _WIN32
    // 如果等待一秒还没有结束，则认为是有问题的，此时强制结束线程
    if (WAIT_OBJECT_0 != WaitForSingleObject(g_cat_monitorHandle, 1000))
    {
        TerminateThread(g_cat_monitorHandle, 0);
    }
    CloseHandle(g_cat_monitorHandle);
#else
    pthread_join(g_cat_monitorHandle, NULL);
#endif // _WIN32
}

