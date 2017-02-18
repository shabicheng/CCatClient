#include "sds.h"
#ifndef CATCLIENTCONFIG_H
#define CATCLIENTCONFIG_H



typedef struct _CatClientConfig
{
    sds domain;
    sds selfHost;
    sds serverHost;
    unsigned short serverPort;
    int serverNum;
    sds * serverAddresses;
    int messageEnableFlag;
    int messageQueueSize;
    int maxChildSize;
    int maxContextElementSize;
    int logFlag;
    int logSaveFlag;
    unsigned int logLevel;
    sds configDir;
    sds dataDir;
    sds indexFileName;
}CatClientConfig;

extern CatClientConfig g_config;


void initCatClientConfig();

#endif//CATCLIENTCONFIG_H