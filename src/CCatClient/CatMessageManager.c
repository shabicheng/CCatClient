#include "CatMessageManager.h"
#include "CatClientConfig.h"
#include "CLog.h"
#include "CatContext.h"
#include "CatMessigeIdHelper.h"
#include "CatMessageSender.h"
#include "CatMessageMergeAndEncode.h"
#include "anet.h"

CatMessageManager g_cat_messageManager = {0};

void catMessageManagerAdd(CatMessage * message)
{
    catContextAdd(message);
}

void catMessageManagerEndTrans(CatTransaction * message)
{
    if (catContextEndTrans(message))
    {
        resetCatContext();
    }
}

void catMessageManagerFlush(CatRootMessage * rootMsg)
{
    //CatRootMessage * pRootMsg = getContextMessageTree();
    if (NULL == rootMsg->m_messageId)
    {
        rootMsg->m_messageId = getNextMessageId();
    }
    if (isCatSenderEnable() && g_config.messageEnableFlag)
    {
        if (sendRootMessage(rootMsg) != 1)
        {
            deleteCatRootMessage(rootMsg);
        }
    }
    else
    {
        deleteCatRootMessage(rootMsg);
        ++g_cat_messageManager.m_throttleTimes;
        if (g_cat_messageManager.m_throttleTimes == 1 || g_cat_messageManager.m_throttleTimes % 10000 == 0)
        {
            INNER_LOG(CLOG_WARNING, "Cat Message is throttled! Times: %d", g_cat_messageManager.m_throttleTimes);
        }
    }
}

void catMessageManagerInitialize(const char * domain, const char * hostName)
{
    g_cat_messageManager.m_domain = sdsnew(domain);
    catChecktPtr(g_cat_messageManager.m_domain);

    g_cat_messageManager.m_hostname = sdsnew(hostName);
    catChecktPtr(g_cat_messageManager.m_hostname);
    // @todo for wyp
    // ip地址好像是16进制的?
    g_cat_messageManager.m_ip = sdsnewEmpty(64);
    catChecktPtr(g_cat_messageManager.m_ip);
    anetResolveIP(NULL, NULL, g_cat_messageManager.m_ip, 64);
	
	// @todo for wyp, cannot find self ip in ubuntu
	if (g_cat_messageManager.m_ip[0] == '\0')
	{
		sdscpy(g_cat_messageManager.m_ip, "192.168.110.161");
	}

    g_cat_messageManager.m_ipX = sdsnewEmpty(64);
    catChecktPtr(g_cat_messageManager.m_ipX);
    anetResolveIPHex(NULL, NULL, g_cat_messageManager.m_ipX, 64);
	
	// @todo for wyp, cannot find self ip in ubuntu
	if (g_cat_messageManager.m_ipX[0] == '\0')
	{
		sdscpy(g_cat_messageManager.m_ipX, "c0a86ea1");
	}


}

void catMessageManagerDestroy()
{
    sdsfree(g_cat_messageManager.m_domain);
    g_cat_messageManager.m_domain = NULL;
    sdsfree(g_cat_messageManager.m_hostname);
    g_cat_messageManager.m_hostname = NULL;
    sdsfree(g_cat_messageManager.m_ip);
    g_cat_messageManager.m_ip = NULL;
    sdsfree(g_cat_messageManager.m_ipX);
    g_cat_messageManager.m_ipX = NULL;
}

void catMessageManagerStartTrans(CatTransaction * trans)
{
    catContextStartTrans(trans);
}

