#include "CatClientCommon.h"
#include "CLog.h"

void catChecktPtr(void * ptr)
{
    if (ptr == NULL)
    {
        INNER_LOG(CLOG_ERROR, "�ڴ�����ʧ��, ����ʼ�˳�.");
        exit(1);
    }
}
