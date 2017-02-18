
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



    int i = 0;
    for (i = 0; i < 1000; ++i)
    {
        CatTransaction * p5Trans = newTransaction("TopTrans5", "SimpleTrans");
        logError("ErrorInner5", "ddd");
        CatTransaction * p4Trans = newTransaction("TopTrans5", "SimpleTrans");
        logEvent("EventInner4", "hehe", CAT_SUCCESS, "key=value");
        CatTransaction * p3Trans = newTransaction("TopTrans5", "SimpleTrans");
        logHeartBeat("HBInner3", "aaa", CAT_SUCCESS, "cpu=3.5");
        CatTransaction * p2Trans = newTransaction("TopTrans5", "SimpleTrans");
        logMetricForCount("countInner2");
        CatTransaction * p1Trans = newTransaction("TopTrans5", "SimpleTrans");
        logErrorTrace("ZCErrorTrace1", "DebugError", "CatTransaction The Test Data.....");
        p1Trans->setStatus((CatMessage *)p1Trans, CAT_SUCCESS);
        p1Trans->setComplete((CatMessage *)p1Trans);
        p2Trans->setStatus((CatMessage *)p2Trans, CAT_SUCCESS);
        p2Trans->setComplete((CatMessage *)p2Trans);
        p3Trans->setStatus((CatMessage *)p3Trans, CAT_SUCCESS);
        p3Trans->setComplete((CatMessage *)p3Trans);
        p4Trans->setStatus((CatMessage *)p4Trans, CAT_SUCCESS);
        p4Trans->setComplete((CatMessage *)p4Trans);
        p5Trans->setStatus((CatMessage *)p5Trans, CAT_SUCCESS);
        p5Trans->setComplete((CatMessage *)p5Trans);
        Sleep(1);
    }
    Sleep(10000);
    catClientDestroy();
    system("pause");
    return 0;

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
//         logErrorTrace("ZCErrorTrace", "DebugError", "The Test Data.....");
//         logError("WYPError", "WYPddd");
//         logEvent("WYPEvent", "WYPhehe", CAT_SUCCESS, "WYPkey=value");
//         logHeartBeat("WYPHB", "WYPaaa", CAT_SUCCESS, "WYPcpu=3.5");
//         logMetricForCount("WYPcount");
//         CatTransaction * pTrans = newTransaction("Trans", "SimpleTrans");
//  
//         logError("ErrorInner", "ddd");
//         logEvent("EventInner", "hehe", CAT_SUCCESS, "key=value");
//         logHeartBeat("HBInner", "aaa", CAT_SUCCESS, "cpu=3.5");
//         logMetricForCount("countInner");
//         pTrans->setStatus((CatMessage *)pTrans, CAT_SUCCESS);
//         pTrans->setComplete((CatMessage *)pTrans);


        CatTransaction * p5Trans = newTransaction("TopTrans5", "SimpleTrans");
        logError("ErrorInner5", "ddd");
        CatTransaction * p4Trans = newTransaction("TopTrans5", "SimpleTrans");
        logEvent("EventInner4", "hehe", CAT_SUCCESS, "key=value");
        CatTransaction * p3Trans = newTransaction("TopTrans5", "SimpleTrans");
        logHeartBeat("HBInner3", "aaa", CAT_SUCCESS, "cpu=3.5");
        CatTransaction * p2Trans = newTransaction("TopTrans5", "SimpleTrans");
        logMetricForCount("countInner2");
        CatTransaction * p1Trans = newTransaction("TopTrans5", "SimpleTrans");
        logErrorTrace("ZCErrorTrace1", "DebugError", "CatTransaction The Test Data.....");
        p1Trans->setStatus((CatMessage *)p1Trans, CAT_SUCCESS);
        p1Trans->setComplete((CatMessage *)p1Trans);
        p2Trans->setStatus((CatMessage *)p2Trans, CAT_SUCCESS);
        p2Trans->setComplete((CatMessage *)p2Trans);
        p3Trans->setStatus((CatMessage *)p3Trans, CAT_SUCCESS);
        p3Trans->setComplete((CatMessage *)p3Trans);
        p4Trans->setStatus((CatMessage *)p4Trans, CAT_SUCCESS);
        p4Trans->setComplete((CatMessage *)p4Trans);
        p5Trans->setStatus((CatMessage *)p5Trans, CAT_SUCCESS);
        p5Trans->setComplete((CatMessage *)p5Trans);

        Sleep(10);
    }
//     printf("Running main() from gtest_main.cc\n");
//     testing::InitGoogleTest(&argc, argv);
//     RUN_ALL_TESTS();

    catClientDestroy();

    system("pause");
    return 0;
}