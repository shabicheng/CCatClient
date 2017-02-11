#include "CatMonitor.h"
#include "ZRMultiThreadUtility.h"
#include "ZRSafeQueue.h"
#include "CatClientConfig.h"
#include "CLog.h"
#include "TimeUtility.h"
#include "CatHeartBeat.h"
#include "CatServerConnManager.h"


static volatile int g_cat_monitorStop = 0;

static void * g_cat_monitorHandle = NULL;

static CatHeartBeat * g_cat_heartbeat = NULL;

#ifdef _WIN32
static DWORD WINAPI catMonitorFun(PVOID para)
#else
static void* catMonitorFun(void* para)
#endif
{
    unsigned long runCount = 0;
    while (!g_cat_monitorStop)
    {
        // ÿʮ���������
        if (++runCount % 10 == 0)
        {
            updateCatServerConn();
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
    // �ȴ��߳��˳�
    g_cat_monitorStop = 1;
    // ɾ���߳�

#ifdef _WIN32
    // ����ȴ�һ�뻹û�н���������Ϊ��������ģ���ʱǿ�ƽ����߳�
    if (WAIT_OBJECT_0 != WaitForSingleObject(g_cat_monitorHandle, 1000))
    {
        TerminateThread(g_cat_monitorHandle, 0);
    }
    CloseHandle(g_cat_monitorHandle);
#else
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
    {
        /* Handle error */
    }

    ts.tv_sec += 1;
    if (pthread_timedjoin_np(g_cat_monitorHandle, NULL, &ts) != 0)
    {
        pthread_cancel(g_cat_monitorHandle);
    }
#endif // _WIN32
}

