#include "CatMessage.h"
#include "TimeUtility.h"



static void addDataPair(CatMessage* message, const char *data)
{
	CatMessageInner * pInner = getInnerMsg(message);
	if (pInner->m_data == NULL)
	{
		pInner->m_data = sdsnew(data);
	}
	else
	{
		sdscat(pInner->m_data, "&");
		sdscat(pInner->m_data, data);
	}
}

static void addData(CatMessage* message, const char *dataKey, const char * dataValue)
{
	CatMessageInner * pInner = getInnerMsg(message);
	if (pInner->m_data == NULL)
	{
		pInner->m_data = sdsnew(dataKey);
		sdscat(pInner->m_data, "=");
		sdscat(pInner->m_data, dataValue);
	}
	else
	{
		sdscat(pInner->m_data, "&");
		sdscat(pInner->m_data, dataKey);
		sdscat(pInner->m_data, "=");
		sdscat(pInner->m_data, dataValue);
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
	CatMessageInner * pInner = getInnerMsg(message);
	if (pInner->m_status != NULL)
	{
		sdsfree(pInner->m_status);
	}
	if (pInner->m_data != NULL)
	{
		sdsfree(pInner->m_data);
	}
	if (pInner->m_type != NULL)
	{
		sdsfree(pInner->m_type);
	}
	if (pInner->m_name != NULL)
	{
		sdsfree(pInner->m_name);
	}
	return pInner;
}

void initCatMessage(CatMessage * pMsg, char msgType, const char *type, const char * name)
{
	CatMessageInner * pInner = getInnerMsg(pMsg);
	memset(pMsg, 0, sizeof(CatMessage) + sizeof(CatMessageInner));
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

