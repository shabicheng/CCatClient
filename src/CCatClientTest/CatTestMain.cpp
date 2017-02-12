
//#include "gtest/gtest.h"
#include "CatClient.h"
#include <ZRLibraryGlobal.h>
#include "TimeUtility.h"
//#include "vld.h"

unsigned long long g_sendCount = 0;

// TEST(CCatClient, BaseTest)
// {
//     while (1)
//     {
//         logError("WYPError", "WYPddd"); 
//         logEvent("WYPEvent", "WYPhehe", CAT_SUCCESS, "WYPkey=value");
//         logHeartBeat("WYPHB", "WYPaaa", CAT_SUCCESS, "WYPcpu=3.5");
//         logMetricForCount("WYPcount");
//         CatTransaction * pTrans = newTransaction("WYPTrans", "SimpleTrans");
//         pTrans->setComplete((CatMessage *)pTrans);
//         Sleep(100);
//     }
// 
//     CatTransaction * pTrans = newTransaction("Trans", "SimpleTrans");
// 
//     logError("ErrorInner", "ddd");
//     logEvent("EventInner", "hehe", CAT_SUCCESS, "key=value");
//     logHeartBeat("HBInner", "aaa", CAT_SUCCESS, "cpu=3.5");
//     logMetricForCount("countInner");
//     pTrans->setComplete((CatMessage *)pTrans);
// }

int main(int argc, char ** argv)
{
    unsigned long long lastTime;
    catClientInit();
    lastTime = GetTime64();
    while (1)
    {
        if (g_sendCount % 100 == 0 && GetTime64() - lastTime >= 1000)
        {
            lastTime = GetTime64();
            printf("Send count %lld \n", g_sendCount);
            g_sendCount = 0;
        }
        ++g_sendCount;
         //if (g_sendCount % 10000 == 0)
         //{
         //    printf("Send count %lld \n", g_sendCount);
         //}
        logError("WYPError", "WYPddd");
        logEvent("WYPEvent", "WYPhehe", CAT_SUCCESS, "WYPkey=value");
        logHeartBeat("WYPHB", "WYPaaa", CAT_SUCCESS, "WYPcpu=3.5");
        logMetricForCount("WYPcount");
        CatTransaction * pTrans = newTransaction("Trans", "SimpleTrans");
 
        logError("ErrorInner", "ddd");
        logEvent("EventInner", "hehe", CAT_SUCCESS, "key=value");
        logHeartBeat("HBInner", "aaa", CAT_SUCCESS, "cpu=3.5");
        logMetricForCount("countInner");
        pTrans->setComplete((CatMessage *)pTrans);
        Sleep(10);
    }
//     printf("Running main() from gtest_main.cc\n");
//     testing::InitGoogleTest(&argc, argv);
//     RUN_ALL_TESTS();

    catClientDestroy();

    system("pause");
    return 0;
}