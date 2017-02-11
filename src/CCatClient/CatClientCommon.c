#include "CatClientCommon.h"
#include "CLog.h"

void catChecktPtr(void * ptr)
{
    if (ptr == NULL)
    {
        INNER_LOG(CLOG_ERROR, "内存申请失败, 程序开始退出.");
        exit(1);
    }
}
