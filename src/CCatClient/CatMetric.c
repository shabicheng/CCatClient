#include "CatMetric.h"

#include "CatMessageManager.h"


static void setMetricComplete(CatMessage * message)
{
	CatMessageInner * pInner = getInnerMsg(message);
	pInner->setCompleteFlag(message, 1);
	catMessageManagerAdd(message);
}


CatMetric * createCatMetric(const char *type, const char * name)
{
	CatMessageInner * pMetricInner = malloc(sizeof(CatMetric) + sizeof(CatMessageInner));
	CatMetric * pMetric = (CatMetric *)((char *)pMetricInner + sizeof(CatMessageInner));
	if (pMetric == NULL)
	{
		return NULL;
	}
	initCatMessage(pMetric, CatMessageType_Metric, type, name);

	// ÉèÖÃ
	// 
	pMetric->setComplete = setMetricComplete;
	return pMetric;
}
