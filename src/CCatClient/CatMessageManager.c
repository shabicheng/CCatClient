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
        sendRootMessage(rootMsg);
    }
    else
    {
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
    g_cat_messageManager.m_hostname = sdsnew(hostName);
    // @todo for wyp
    // ip地址好像是16进制的?
    g_cat_messageManager.m_ip = sdsnewlen(NULL, 64);
    anetResolveIP(NULL, NULL, g_cat_messageManager.m_ip, 64);
    g_cat_messageManager.m_ipX = sdsnewlen(NULL, 64);
    anetResolveIPHex(NULL, NULL, g_cat_messageManager.m_ipX, 64);

}

void catMessageManagerStartTrans(CatTransaction * trans)
{
    catContextStartTrans(trans);
}

