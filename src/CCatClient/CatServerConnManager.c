#include "CatServerConnManager.h"
#include "ZRMultiThreadUtility.h"
#include "ZRSafeQueue.h"
#include "CatClientConfig.h"
#include "anet.h"
#include "CLog.h"
#include "CatMessageManager.h"


static sds g_server_responseBody = NULL;
static sds g_server_requestBuf = NULL;
static sds g_server_ips[64] = {0};
static unsigned short g_server_ports[64] = {0};
static volatile int g_server_count = 0;
static volatile int g_server_activeId = -1;

static ZRCRITICALSECTION g_server_lock = NULL;


extern CatMessageManager g_cat_messageManager;


extern char g_cat_send_ip[64];
extern unsigned short g_cat_send_port;
extern int g_cat_send_fd;

static int tryConnBestServer()
{
    int newFd = -1;
    int oldFd = -1;
    // @todo for wyp Ä¿Ç°²ßÂÔ¾ÍÊÇ¼òµ¥µÄÈ¥Á¬µÚÒ»¸ö·þÎñÆ÷
    if (g_server_activeId == 0)
    {
        return 1;
    }
    int ipValidNum = g_server_activeId;
    // Èç¹ûµ±Ç°Ê§Ð§£¬Ôò´ÓËùÓÐ·þÎñÆ÷ÁÐ±íÖÐÕÒµ½Ò»¸ö¿ÉÓÃ·þÎñÆ÷
    if (ipValidNum < 0)
    {
        ipValidNum = g_server_count;
    }

	int i = 0;
    for (; i < ipValidNum; ++i)
    {
        // @todo ,Õâ±ßÄ¬ÈÏÏÈÉèÖÃ³É×èÈû
        newFd = anetTcpConnect(NULL, g_server_ips[i], g_server_ports[i]);
        if (newFd > 0)
        {
            g_server_activeId = i;
            strcpy(g_cat_send_ip, g_server_ips[i]);
            g_cat_send_port = g_server_ports[i];
            oldFd = g_cat_send_fd;
            g_cat_send_fd = newFd;
            if (oldFd > 0)
            {
                anetClose(oldFd);
            }
            return 1;
        }
    }
    return 0;

}



static void updateCatActiveConnIndex()
{
    if (g_server_activeId < 0)
    {
        return;
    }
    int i = 0;
    for (i = 0; i < g_server_count; ++i)
    {
        if (strcmp(g_server_ips[i], g_cat_send_ip) == 0 && g_server_ports[i] == g_cat_send_port)
        {
            g_server_activeId = i;
            return;
        }
    }
    g_server_activeId = -1;
}


// resolve ip such as "192.168.110.111:9090"
static int resolveIpPortStr(sds ipPortStr, sds * ip, unsigned short * port)
{
    size_t i = 0;
    
    for (; i < sdslen(ipPortStr); ++i)
    {
        if (ipPortStr[i] == ':')
        {
            *ip = sdsnewlen(ipPortStr, i);
            long lPort = atol(ipPortStr + i + 1);
            if (lPort <= 0 || lPort > 65536)
            {
                *port = 80;
            }
            else
            {
                *port = (unsigned short)lPort;
            }
            return 1;
        }
    }
    return 0;
}

// sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);
// void sdsfreesplitres(sds *tokens, int count
static int resolveServerIps()
{
    int i = 0;
    int count = 0;
    int validIpCount = 0;
    sds * spIpPorts = sdssplitlen(g_server_responseBody, sdslen(g_server_responseBody), ";", 1, &count);

    if (spIpPorts != NULL && count > 0)
    {
    }
    else
    {
        return 0;
    }


    ZRCS_ENTER(g_server_lock);

    // Èç¹ûÕâ´ÎÇëÇóµ½IP£¬ÐèÒª°ÑÉÏ´Î±£´æµÄÉ¾³ý
    for (i = 0; i < g_server_count; ++i)
    {
        sdsfree(g_server_ips[i]);
        g_server_ips[i] = NULL;
    }

    for (i = 0; i < count && i < 64; ++i)
    {
        if (resolveIpPortStr(spIpPorts[i], g_server_ips + validIpCount, g_server_ports + validIpCount) > 0)
        {
            ++validIpCount;
        }
    }
    sdsfreesplitres(spIpPorts, count);
    g_server_count = validIpCount;
    ZRCS_LEAVE(g_server_lock);
    return validIpCount;
}

static int getRouterFromServer(char * hostName, unsigned short port, char * domain)
{
//     // @debug
//     return 0;
//     // @debug end
    int sockfd = -1;
    if (g_server_requestBuf == NULL)
    {
        g_server_requestBuf = sdsnewEmpty(1024);
        catChecktPtr(g_server_requestBuf);
    }
    char destIP[128];
    if (anetResolveIP(NULL, hostName, destIP, 128) == ANET_ERR)
    {
        return 0;
    }
    sockfd = anetTcpConnect(NULL, destIP, port);
    if (sockfd < 0)
    {
        INNER_LOG(CLOG_WARNING, "Ïò·þÎñÆ÷ %s %d·¢ÆðÁ¬½ÓÊ§°Ü.", destIP, port);
        return 0;
    }
    sdsclear(g_server_requestBuf);
    if (port == 80)
    {

        g_server_requestBuf = sdscatprintf(g_server_requestBuf, "Get http://%s/cat/s/router?domain=%s HTTP/1.0\r\n", hostName, domain);
    }
    else
    {
        g_server_requestBuf = sdscatprintf(g_server_requestBuf, "Get http://%s:%d/cat/s/router?domain=%s HTTP/1.0\r\n", hostName, (int)port, domain);
    }
    g_server_requestBuf = sdscatprintf(g_server_requestBuf, "Host %s\r\n", hostName);
    g_server_requestBuf = sdscatprintf(g_server_requestBuf, "Connection: close\r\n\r\n");
    int status = anetWrite(sockfd, g_server_requestBuf, sdslen(g_server_requestBuf));
    if (status == ANET_ERR)
    {
        anetClose(sockfd);
        return 0;
    }
    char resp[1024];
    status = anetRead(sockfd, resp, 1023);
    if (status == ANET_ERR || status < 4)
    {
        anetClose(sockfd);
        return 0;
    }
    resp[status] = '\0';
    char *t = strstr(resp, "\r\n\r\n");
    if (!t)
    {
        anetClose(sockfd);
        return 0;
    }
    char *body = t + 4;
    if (body[0] == '\0')
    {
        anetClose(sockfd);
        return 0;
    }
    anetClose(sockfd);
    if (g_server_responseBody == NULL)
    {
        g_server_responseBody = sdsnewEmpty(1024);
        catChecktPtr(g_server_responseBody);
    }
    else
    {
        // Èç¹ûÁ½´ÎÏàÍ¬¾Í²»ÐèÒªÇ¿ÐÐ¸üÐÂ
        if (strcmp(g_server_responseBody, body) == 0)
        {
            return g_server_count;
        }
    }
    INNER_LOG(CLOG_INFO, "Ïò·þÎñÆ÷²éÑ¯µ½¿ÉÓÃ·þÎñÆ÷ÁÐ±í %s .", body);
    sdscpy(g_server_responseBody, body);
    return resolveServerIps();
}


int recoverCatServerConn()
{
    anetClose(g_cat_send_fd);
    g_cat_send_fd = -1;
    g_server_activeId = -1;
    if (!tryConnBestServer())
    {
        INNER_LOG(CLOG_WARNING, "Ö±½Ó»Ö¸´Óë·þÎñÆ÷Á¬½ÓÊ§°Ü, ³¢ÊÔ¸üÐÂÂ·ÓÉ.");
        if (!updateCatServerConn())
        {
            INNER_LOG(CLOG_ERROR, "ÔÙ´Î³¢ÊÔÊ§°Ü£¬·þÎñÆ÷µ±Ç°²»¿ÉÓÃ.");
            return 0;
        }
    }
    return 1;
}

int initCatServerConnManager()
{
    g_server_lock = ZRCreateCriticalSection();

    // ÏÈ´ÓÅäÖÃÄÇ±ß¶Á¹ýÀ´³õÊ¼µÄ·þÎñÆ÷ÅäÖÃ£¬ÕâÑù¼´Ê¹ÔÚrouter·þÎñÆ÷²»¿ÉÓÃµÄÇé¿öÏÂÒ²¿ÉÒÔÁ¬½Óserver
    g_server_count = g_config.serverNum;
    if (g_server_count > 64)
    {
        g_server_count = 64;
    }
    int i = 0;
    int validCount = 0;
    for (; i < g_server_count; ++i)
    {
        if (resolveIpPortStr(g_config.serverAddresses[i], g_server_ips + validCount, g_server_ports + validCount))
        {
            ++validCount;
        }
    }
    g_server_count = validCount;

    return updateCatServerConn();
}

void clearCatServerConnManager()
{
    int i = 0;
    ZRDeleteCriticalSection(g_server_lock);
    if (g_cat_send_fd > 0)
    {
        anetClose(g_cat_send_fd);
    }
    for (i = 0; i < g_server_count; ++i)
    {
        sdsfree(g_server_ips[i]);
        g_server_ips[i] = NULL;
    }
}
int updateCatServerConn()
{
    // @todo for wyp Õâ±ß²»ÖªµÀ¾ßÌåÁ¬ÄÇ¸ö¶Ë¿Ú¡£¡£¡£ÊÇÅäÖÃ»¹ÊÇÓÃ»§Ìî»¹ÊÇÄ¬ÈÏ£¿
    int rst = getRouterFromServer(g_cat_messageManager.m_hostname, 8080, g_cat_messageManager.m_domain);
    //if (rst > 0)
    {
        updateCatActiveConnIndex();
        if (tryConnBestServer() == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
