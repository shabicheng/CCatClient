
#ifndef CATCLIENT_H
#define CATCLIENT_H
#include "CatClientCommon.h"
#include "CatHeartBeat.h"
#include "CatEvent.h"
#include "CatTrace.h"
#include "CatMetric.h"
#include "CatTransaction.h"

#define CAT_OK 1
#define CAT_ERR 0



#ifdef __cplusplus

extern "C"
{

#endif

    CATCLIENT_EXPORT int catClientInit();
    CATCLIENT_EXPORT int catClientDestroy();
    CATCLIENT_EXPORT void logError(const char * msg, const char * errStr);
    CATCLIENT_EXPORT void logEvent(const char * type, const char * name, const char * status, const char * nameValuePairs);
    CATCLIENT_EXPORT void logHeartBeat(const char * type, const char * name, const char * status, const char * nameValuePairs);
    CATCLIENT_EXPORT void logMetric(const char * name, const char * key, const char * value);
    CATCLIENT_EXPORT void logMetricForCount(const char * name);
    CATCLIENT_EXPORT void logMetricForCountQuantity(const char * name, int quantity);
    CATCLIENT_EXPORT void logMetricForDutation(const char * name, unsigned long long durationUs);
    CATCLIENT_EXPORT void logMetricForSum(const char * name, double value);
    CATCLIENT_EXPORT void logMetricForSumQuantity(const char * name, double value, int quantity);
#define logTrace(type, name, status, nameValuePairs) logTraceWithCodeLocation((type), (name), (status), (nameValuePairs), \
    __FILE__, __FUNCTION__, __LINE__)
    CATCLIENT_EXPORT void logTraceWithCodeLocation(const char * type, const char * name, const char * status,
        const char * nameValuePairs, const char * fileName, const char * funcationName, int lineNo);

#define logErrorTrace(type, name, data) logErrorWithCodeLocation((type), (name), (data), \
    __FILE__, __FUNCTION__, __LINE__)
    CATCLIENT_EXPORT void logErrorWithCodeLocation(const char * type, const char * name,
                                                   const char * data, const char * fileName, const char * funcationName, int lineNo);

    CATCLIENT_EXPORT CatEvent *  newEvent(const char * type, const char * name);
    CATCLIENT_EXPORT CatTrace * newTrace(const char * type, const char * name);
    CATCLIENT_EXPORT CatMetric * newMetric(const char * type, const char * name);
    CATCLIENT_EXPORT CatHeartBeat * newHeartBeat(const char * type, const char * name);
    CATCLIENT_EXPORT CatTransaction * newTransaction(const char * type, const char * name);

#ifdef __cplusplus

}
#endif

#endif //CATCLIENT_H
