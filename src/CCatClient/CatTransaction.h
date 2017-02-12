#ifndef CATTRANSACTION_h__
#define CATTRANSACTION_h__


#include "CatMessage.h"
#include "ZRStaticQueue.h"



typedef struct _CatTranscation CatTransaction;

typedef struct _CatTranscationInner
{
	ZRStaticQueue * m_children;
	unsigned long long m_durationStart; // ¸ß¾«¶ÈÊ±¼ä£¬¾«È·µ½ÄÉÃë£¬Õâ±ßÏÈÓÃGetTime64 * 1000 * 1000ÊµÏÖ×Å
	unsigned long long m_durationUs; 

	// Õâ±ßÐèÒªÓëCatMessageInner±£³ÖÑÏ¸ñÒ»ÖÂ
	char m_msgType;
	char m_unused[3]; //4×Ö½Ú¶ÔÆë
	sds m_type;
	sds m_name;
	sds m_status;
	sds m_data;
	unsigned long long m_timeStamp; //Í³Ò»º¬Òå£¬sec*1000 + ms
	int m_completeFlag;
	void(*setCompleteFlag)  (CatMessage* message, int completeFlag);
	void *(*clear)   (CatMessage* message);


}CatTransactionInner;


struct _CatTranscation
{
	void(*addDataPair)   (CatMessage* message, const char *data);
	void(*addData)   (CatMessage* message, const char *dataKey, const char * dataValue);
	void(*setStatus) (CatMessage* message, const char *status);
	void(*setComplete)  (CatMessage* message);
	void *(*clear)   (CatMessage* message);
    void(*addChild)    (CatTransaction* message, CatMessage* childMsg);
    ZRStaticQueue *(*getChildren)    (CatTransaction* message);
};


#define getInnerTrans(pMsg) ((CatTransactionInner*)(((char*)(pMsg)) - sizeof(CatTransactionInner)))

CatTransaction * createCatTransaction(const char *type, const char * name);

CatTransaction * copyCatTransaction(CatTransaction * pSrcTrans);

unsigned long long getCatTranscationDurationUs(CatTransaction * trans);

static void inline setCatTranscationDurationUs(CatTransaction * trans, unsigned long long durationUs)
{
    CatTransactionInner * pInner = getInnerTrans(trans);
    pInner->m_durationUs = durationUs;
}



#endif//CATTRANSACTION_h__
