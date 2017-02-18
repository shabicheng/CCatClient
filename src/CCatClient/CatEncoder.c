#include "CatEncoder.h"
#include "CatMessageManager.h"
#include "CLog.h"
#include "CatTranscationHelper.h"
#include "TimeUtility.h"

#define POLICY_DEFAULT 0
#define POLICY_WITHOUT_STATUS 1
#define POLICY_WITH_DURATION 2

#define CAT_ENCODE_VERSION "PT1"
#define CAT_TAB '\t'
#define CAT_EL '\n'



extern CatMessageManager g_cat_messageManager;



static inline sds sdscatwithnull(sds s, const char * buf)
{
    return sdscat(s, buf == NULL ? "null" : buf);
}


static inline sds sdscatwithdefault(sds s, const char * buf, const char * defaultStr)
{
    return sdscat(s, buf == NULL ? defaultStr : buf);
}

int catEncodeHeader(CatRootMessage * pRootMsg, sds * buf)
{
    sds tmpBuf = *buf;
    int count = sdslen(tmpBuf);


    tmpBuf = sdscatwithnull(tmpBuf, CAT_ENCODE_VERSION);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, g_cat_messageManager.m_domain);
    //tmpBuf = sdscatwithnull(tmpBuf, "cat");
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, g_cat_messageManager.m_hostname);
    //tmpBuf = sdscatwithnull(tmpBuf, "ubunto");
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, g_cat_messageManager.m_ip);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pRootMsg->m_threadGroupName);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pRootMsg->m_threadId);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pRootMsg->m_threadName);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pRootMsg->m_messageId);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pRootMsg->m_parentMessageId);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pRootMsg->m_rootMessageId);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pRootMsg->m_sessionToken);
    tmpBuf = sdscatchar(tmpBuf, CAT_EL);



    *buf = tmpBuf;
    return sdslen(tmpBuf) - count;
}

int catEncodeLine(CatMessage * pMsg, sds * buf, char type, int policy)
{
    sds tmpBuf = *buf;
    int count = sdslen(tmpBuf);
    CatMessageInner * pMsgInner = getInnerMsg(pMsg);


    tmpBuf = sdscatchar(tmpBuf, type);

    if (type == 'T' && isCatTransaction(pMsg)) 
    {
        unsigned long long durationMs = getCatTranscationDurationUs((CatTransaction *)pMsg) / 1000;

        tmpBuf = sdscatprintf(tmpBuf, "%s", GetCatTimeString(getCatMessageTimeStamp(pMsg) + durationMs));
    }
    else 
    {
        tmpBuf = sdscatprintf(tmpBuf, "%s", GetCatTimeString(getCatMessageTimeStamp(pMsg)));
    }
    //printf("Now Time %s\n", GetCatTimeString(getCatMessageTimeStamp(pMsg)));

    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pMsgInner->m_type);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    tmpBuf = sdscatwithnull(tmpBuf, pMsgInner->m_name);
    tmpBuf = sdscatchar(tmpBuf, CAT_TAB);

    if (policy != POLICY_WITHOUT_STATUS)
    {
        // if status is null, set status to "DefaultStatus"
        tmpBuf = sdscatwithdefault(tmpBuf, pMsgInner->m_status, "DefaultStatus");
        tmpBuf = sdscatchar(tmpBuf, CAT_TAB);


        if (policy == POLICY_WITH_DURATION && isCatTransaction(pMsg))
        {
            unsigned long long durationUs = getCatTranscationDurationUs((CatTransaction *)pMsg);
            tmpBuf = sdscatprintf(tmpBuf, "%lldus\t", durationUs);
        }

        tmpBuf = sdscatwithnull(tmpBuf, pMsgInner->m_data);
        tmpBuf = sdscatchar(tmpBuf, CAT_TAB);
    }

    tmpBuf = sdscatchar(tmpBuf, CAT_EL);



    *buf = tmpBuf;
    return sdslen(tmpBuf) - count;

}

int catEncodeBody(CatMessage * pMsg, sds * buf)
{
    sds tmpBuf = *buf;
    int count = sdslen(tmpBuf);


    if (isCatTransaction(pMsg))
    {
        CatTransaction * transaction = (CatTransaction *)pMsg;
        ZRStaticQueue * children = transaction->getChildren(transaction);

        if (isZRStaticQueueEmpty(children)) 
        {
            return catEncodeLine(pMsg, buf, 'A', POLICY_WITH_DURATION);
        }
        else 
        {
            int count = 0;
            size_t len = getZRStaticStackSize(children);

            count += catEncodeLine(pMsg, buf, 't', POLICY_WITHOUT_STATUS);
			size_t i = 0;
            for (; i < len; i++) 
            {
                CatMessage * child = getZRStaticStackByIndex(children, i);

                if (child != NULL)
                {
                    count += catEncodeBody(child, buf);
                }
            }

            count += catEncodeLine(pMsg, buf, 'T', POLICY_WITH_DURATION);

            return count;
        }
    }
    else if (isCatEvent(pMsg)) 
    {
        return catEncodeLine(pMsg, buf, 'E', POLICY_DEFAULT);
    }
    else if (isCatTrace(pMsg)) 
    {
        return catEncodeLine(pMsg, buf, 'L', POLICY_DEFAULT);
    }
    else if (isCatMetric(pMsg)) 
    {
        return catEncodeLine(pMsg, buf, 'M', POLICY_DEFAULT);
    }
    else if (isCatHeartBeat(pMsg)) 
    {
        return catEncodeLine(pMsg, buf, 'H', POLICY_DEFAULT);
    }
    else 
    {
        INNER_LOG(CLOG_ERROR, "Unsupported message type: %s.", getCatMessageType(pMsg));
    }

    return 0;
}




sds catEncodeMessage(CatRootMessage * pRootMsg, sds buf)
{
    int count = 0;
    buf = sdscatlen(buf, "0000", 4);

    count += catEncodeHeader(pRootMsg, &buf);

    if (pRootMsg->m_rootMsg != NULL)
    {
        count += catEncodeBody(pRootMsg->m_rootMsg, &buf);
    }

    buf[0] = (count >> 24) & 0xFF;
    buf[1] = (count >> 16) & 0xFF;
    buf[2] = (count >> 8) & 0xFF;
    buf[3] = (count) & 0xFF;



    return buf;
}

