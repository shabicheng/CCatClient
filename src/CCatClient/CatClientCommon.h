#ifndef CATCLIENTCOMMON_H
#define CATCLIENTCOMMON_H


#ifdef WIN32

#ifdef  CCATCLIENT_EXPORTS
#define  CATCLIENT_EXPORT __declspec(dllexport)
#else
#define  CATCLIENT_EXPORT __declspec(dllimport)
#endif

#else

#define CATCLIENT_EXPORT

#endif

#ifdef WIN32
#define inline __inline
#endif


#define CAT_SUCCESS "SUCCESS"
#define CAT_ERROR "ERROR"
#define CAT_FAIL "FAIL"

// �������������ڴ棬������������ΪNULL�������ǰ�����ڴ�����
// Ŀǰ�ȼ�ʵ�֣�ֻ�Ǵ�ӡ��Ȼ���˳�
// δ����Ҫ�ڼ�⵽�쳣ʱ��ֱ�ӳ��Է���һ���ڴ���������Ϣ��������
// ע�⣬������Ϣһ��Ҫ����������ڴ棬���ֱ�ӵ��õ�ǰ���õ�socket����
void catChecktPtr(void *);

inline unsigned long long catTrimToHour(unsigned long long timeMs)
{
    return timeMs / (3600 * 1000);
}

#ifdef WIN32
#define CATTHREADLOCAL __declspec(thread)
#elif defined(__linux__)
#define CATTHREADLOCAL __thread
#else
#define CATTHREADLOCAL 
#endif

#endif//CATCLIENTCOMMON_H