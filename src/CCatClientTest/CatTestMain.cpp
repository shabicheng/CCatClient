
#include "gtest/gtest.h"
#include <CatClient.h>
#include <ZRLibraryGlobal.h>


TEST(CCatClient, BaseTest)
{
    logError("Error", "ddd");
    logEvent("Event", "hehe", CAT_SUCCESS, "key=value");
    logHeartBeat("HB", "aaa", CAT_SUCCESS, "cpu=3.5");
    logMetricForCount("count");
    while (1)
    {
        Sleep(1000);
    }

//     CatTransaction * pTrans = newTransaction("Trans", "SimpleTrans");
// 
//     logError("ErrorInner", "ddd");
//     logEvent("EventInner", "hehe", CAT_SUCCESS, "key=value");
//     logHeartBeat("HBInner", "aaa", CAT_SUCCESS, "cpu=3.5");
//     logMetricForCount("countInner");
//     pTrans->setComplete((CatMessage *)pTrans);
}

int main(int argc, char ** argv)
{

    catClientInit();

    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    catClientDestroy();

    system("pause");
    return 0;
}