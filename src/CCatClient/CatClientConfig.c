#include "CatClientConfig.h"
#include <string.h>
#include <stdlib.h>
#include "CLog.h"
#include "anet.h"
CatClientConfig g_config;


extern unsigned char g_log_permissionOpt;

// @todo for wyp
// 配置文件加载以及一些初始化参数自己来写
void initCatClientConfig()
{
    memset(&g_config, 0, sizeof(g_config));
    g_config.domain = sdsnew("CCatClient");
    g_config.selfHost = sdsnewEmpty(128);
    if (anetGetHost(NULL, g_config.selfHost, 128) == ANET_ERR)
    {
        sdscpy(g_config.selfHost, "CUnknownHost");
    }
    g_config.serverHost = sdsnew("192.168.110.159");
    g_config.serverPort = 8080;
    g_config.serverNum = 2;
    g_config.serverAddresses = (sds *)malloc(g_config.serverNum * sizeof(sds));
    g_config.serverAddresses[0] = sdsnew("192.168.110.159:2280");
    g_config.serverAddresses[1] = sdsnew("127.0.0.1:2280");
    g_config.messageEnableFlag = 1;
    g_config.messageQueueSize = 256;
    g_config.maxContextElementSize = 256;
    g_config.maxChildSize = 256;
    g_config.logFlag = 1;
    g_config.logSaveFlag = 1;
    g_config.logLevel = CLOG_ALL;
    g_config.configDir = sdsnew("./");
    g_config.dataDir = sdsnew("./");
    g_config.indexFileName = sdsnew("client.index");


    g_log_permissionOpt = g_config.logLevel;
}

