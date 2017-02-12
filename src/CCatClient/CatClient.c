#include "CatClient.h"
#include "CatClientConfig.h"
#include "CatMessigeIdHelper.h"
#include "CatMessageManager.h"
#include "CatServerConnManager.h"
#include "CatMonitor.h"
#include "CatMessageSender.h"
#include "CatMessageMergeAndEncode.h"
#include "ZRLibraryGlobal.h"
#include "CatContext.h"
//#include "vld.h"

int catClientInit()
{

#ifdef WIN32
    WSADATA  Ws;
    if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
    {
        return -1;
    }
#endif
    initCatClientConfig();
    catMessageManagerInitialize(g_config.domain, g_config.host);
    initMessageIdHelper();
    initCatSenderThread();
    initCatMergeAndEncodeThread();
    initCatServerConnManager();
    initCatMonitor();
    return 1;
}

int catClientDestroy()
{
    clearCatMonitor();
    catMessageManagerDestroy();
    clearCatMergeAndEncodeThread();
    clearCatSenderThread();
    clearCatServerConnManager();
    flushMessageIdMark();
    return 1;
}

void logError(const char * msg, const char * errStr)
{
    logEvent("Exception", msg, "ERROR", errStr);
}

void logEvent(const char * type, const char * name, const char * status, const char * nameValuePairs)
{
    CatEvent * event = newEvent(type, name);
    catChecktPtr(event);
    if (nameValuePairs != NULL)
    {
        event->addDataPair(event, nameValuePairs);
    }
    event->setStatus(event, status);
    event->setComplete(event);

}

void logHeartBeat(const char * type, const char * name, const char * status, const char * nameValuePairs)
{
    CatHeartBeat * hb = newHeartBeat(type, name);
    catChecktPtr(hb);
    if (nameValuePairs != NULL)
    {
        hb->addDataPair(hb, nameValuePairs);
    }
    hb->setStatus(hb, status);
    hb->setComplete(hb);
}

void logMetric(const char * name, const char * key, const char * value)
{
    CatMetric * metric = newMetric("", name);
    catChecktPtr(metric);
    if (key != NULL && value != NULL)
    {
        metric->addData(metric, key, value);
    }
    metric->setStatus(metric, CAT_SUCCESS);
    metric->setComplete(metric);
}

void logMetricForCount(const char * name)
{
    logMetric(name, "C", "1");
}

void logMetricForCountQuantity(const char * name, int quantity)
{
    sds val = sdsfromlonglong(quantity);
    catChecktPtr(val);
    logMetric(name, "C", val);
    sdsfree(val);
}

void logMetricForDutation(const char * name, unsigned long long durationUs)
{

    sds val = sdsfromlonglong(durationUs);
    catChecktPtr(val);
    logMetric(name, "T", val);
    sdsfree(val);
}

void logMetricForSum(const char * name, double value)
{
    char buf[256];
    snprintf(buf, 256, "%.2llf", value);
    buf[255] = '\0';
    logMetric(name, "S", buf);

}

void logMetricForSumQuantity(const char * name, double value, int quantity)
{
    char buf[256];
    snprintf(buf, 256, "%d,%.2llf", quantity, value);
    buf[255] = '\0';
    // @todo for wyp 这边是S,C 不应该是C,S么？
    logMetric(name, "S,C", buf);
}

void logTraceWithCodeLocation(const char * type, const char * name, const char * status, const char * nameValuePairs, const char * fileName, const char * funcationName, int lineNo)
{
    CatTrace * trace = newTrace(type, name);
    catChecktPtr(trace);
    trace->addData(trace, "fileName", fileName);
    trace->addData(trace, "funcationName", funcationName);
    sds lineStr = sdsfromlonglong(lineNo);
    catChecktPtr(lineStr);
    trace->addData(trace, "lineNo", lineStr);
    sdsfree(lineStr);
    if (nameValuePairs != NULL)
    {
        trace->addDataPair(trace, nameValuePairs);
    }
    trace->setStatus(trace, status);
    trace->setComplete(trace);
}

CatEvent * newEvent(const char * type, const char * name)
{
    getCatContext();
    CatEvent * event = createCatEvent(type, name);
    catChecktPtr(event);
    return event;
}

CatTrace * newTrace(const char * type, const char * name)
{
    getCatContext();
    CatTrace * trace = createCatTrace(type, name);
    catChecktPtr(trace);
    return trace;

}

CatMetric * newMetric(const char * type, const char * name)
{

    getCatContext();
    CatMetric * metric = createCatMetric(type, name);
    catChecktPtr(metric);
    return metric;
}

CatHeartBeat * newHeartBeat(const char * type, const char * name)
{
    getCatContext();
    CatHeartBeat * hb = createCatHeartBeat(type, name);
    catChecktPtr(hb);
    return hb;

}

CatTransaction * newTransaction(const char * type, const char * name)
{

    getCatContext();
    CatTransaction * trans = createCatTransaction(type, name);
    catMessageManagerStartTrans(trans);
    catChecktPtr(trans);
    return trans;
}

