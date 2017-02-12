#include "CatMessage.h"
#include "TimeUtility.h"

// unsigned long long g_dbg_newCount = 0;
// unsigned long long g_gdb_delCount = 0;


static void addDataPair(CatMessage* message, const char *data)
{
    CatMessageInner * pInner = getInnerMsg(message);
    if (pInner->m_data == NULL)
    {
        pInner->m_data = sdsnew(data);
    }
    else
    {
        pInner->m_data = sdscat(pInner->m_data, "&");
        pInner->m_data = sdscat(pInner->m_data, data);
    }
}

static void addData(CatMessage* message, const char *dataKey, const char * dataValue)
{
    CatMessageInner * pInner = getInnerMsg(message);
    if (pInner->m_data == NULL)
    {
        pInner->m_data = sdsnew(dataKey);
        pInner->m_data = sdscat(pInner->m_data, "=");
        pInner->m_data = sdscat(pInner->m_data, dataValue);
    }
    else
    {
        pInner->m_data = sdscat(pInner->m_data, "&");
        pInner->m_data = sdscat(pInner->m_data, dataKey);
        pInner->m_data = sdscat(pInner->m_data, "=");
        pInner->m_data = sdscat(pInner->m_data, dataValue);
    }
}

static void setStatus(CatMessage* message, const char *status)
{
    CatMessageInner * pInner = getInnerMsg(message);
    if (pInner->m_status == NULL)
    {
        pInner->m_status = sdsnew(status);
    }
    else
    {
        sdscpy(pInner->m_status, status);
    }
}

static void setComplete(CatMessage* message)
{
    CatMessageInner * pInner = getInnerMsg(message);
    pInner->m_completeFlag = 1;
}

static void setCompleteFlag(CatMessage* message, int completeFlag)
{
    CatMessageInner * pInner = getInnerMsg(message);
    pInner->m_completeFlag = completeFlag;
}

static void * clear(CatMessage* message)
{
    //++g_gdb_delCount;
    CatMessageInner * pInner = getInnerMsg(message);
    if (pInner->m_status != NULL)
    {
        sdsfree(pInner->m_status);
        pInner->m_status = NULL;
    }
    if (pInner->m_data != NULL)
    {
        sdsfree(pInner->m_data);
        pInner->m_data = NULL;
    }
    if (pInner->m_type != NULL)
    {
        sdsfree(pInner->m_type);
        pInner->m_type = NULL;
    }
    if (pInner->m_name != NULL)
    {
        sdsfree(pInner->m_name);
        pInner->m_name = NULL;
    }
    return pInner;
}

void initCatMessage(CatMessage * pMsg, char msgType, const char *type, const char * name)
{
//     ++g_dbg_newCount;
//     if (g_dbg_newCount % 10000 == 0)
//     {
//         printf("%lld %lld ************\n", g_dbg_newCount, g_gdb_delCount);
//     }

    CatMessageInner * pInner = getInnerMsg(pMsg);
    memset(pInner, 0, sizeof(CatMessage) + sizeof(CatMessageInner));
    pInner->m_msgType = msgType;
    pInner->m_timeStampMs = GetTime64();
    pInner->m_type = sdsnew(type);
    pInner->m_name = sdsnew(name);
    pInner->setCompleteFlag = setCompleteFlag;
    pInner->clear = clear;


    pMsg->addData = addData;
    pMsg->addDataPair = addDataPair;
    pMsg->clear = clear;
    pMsg->setComplete = setComplete;
    pMsg->setStatus = setStatus;
}

