#ifndef CATCLIENTCOMMON_H
#define CATCLIENTCOMMON_H

#ifdef WIN32
#define inline __inline
#endif

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