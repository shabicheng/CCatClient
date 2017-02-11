#ifndef CATTRANSACTION_h__
#define CATTRANSACTION_h__


#include "CatMessage.h"
#include "ZRStaticQueue.h"



typedef struct _CatTranscation CatTransaction;

typedef struct _CatTranscationInner
{
	ZRStaticQueue * m_children;
	unsigned long long m_durationStart; // 高精度时间，精确到纳秒，这边先用GetTime64 * 1000 * 1000实现着
	unsigned long long m_durationUs; 

	// 这边需要与CatMessageInner保持严格一致
	char m_msgType;
	char m_unused[3]; //4字节对齐
	sds m_type;
	sds m_name;
	sds m_status;
	sds m_data;
	unsigned long long m_timeStamp; //统一含义，sec*1000 + ms
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

void inline setCatTranscationDurationUs(CatTransaction * trans, unsigned long long durationUs)
{
    CatTransactionInner * pInner = getInnerTrans(trans);
    pInner->m_durationUs = durationUs;
}



#endif//CATTRANSACTION_h__
