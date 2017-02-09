// /*************************************************************************************************
// @Assembly         
// @Author           ZRZC
// @Created          11-17-2015
//
// @Last Modified By ZRZC
// @Last Modified On 11-17-2015
// 
// @copyright        Copyright (c) ZhenRong. All rights reserved.
// @summary          ���̲߳�����һЩ������Դ��װ���ź��������������ٽ���
// **************************************************************************************************/
#ifndef _ZRMULTITHREADUTILITY_
#define _ZRMULTITHREADUTILITY_

#include "ZRLibraryGlobal.h"


//����Ϊ���ֺͲ���ϵͳ��Դ��صĹ��ߺ궨��
#ifdef WIN32
/// * @brief    /�ٽ�����Դ
typedef LPCRITICAL_SECTION ZRCRITICALSECTION;
#define INVALID_CRITSECT NULL
/// *************************************************************************************************
/// * @fn       ZRCreateCriticalSection
/// * @brief    ����������.
/// * @param    spinCount  The spin count.
/// * @returns    ZRCRITICALSECTION.
/// * 
/// * @details  
/// * 
/// * @todo     
/// * 
/// * @author   ZRZC
/// * 
/// * @date     2015/11/20 17:42:27
/// ************************************************************************************************/
inline ZRCRITICALSECTION ZRCreateCriticalSection()
{
	ZRCRITICALSECTION cs = (CRITICAL_SECTION *)malloc(sizeof(CRITICAL_SECTION));
	//assert(cs != INVALID_CRITSECT);
	if (0)
	{
		InitializeCriticalSection(cs);
	}
	else
	{
		InitializeCriticalSectionAndSpinCount(cs, 4000);
	}
	return cs;
}
/// *************************************************************************************************
/// * @fn       ZRDeleteCriticalSection
/// * @brief    ɾ��������
/// * @param    cs  The cs.
/// * 
/// * @details  
/// * 
/// * @todo     
/// * 
/// * @author   ZRZC
/// * 
/// * @date     2015/11/20 17:42:36
/// ************************************************************************************************/
inline void ZRDeleteCriticalSection(ZRCRITICALSECTION cs)
{
	if (cs != INVALID_CRITSECT)
	{
		DeleteCriticalSection(cs);
		free(cs);
		cs = 0;
	}
}

/// * @brief    ����
#define ZRCS_ENTER(cs) EnterCriticalSection(cs)
/// * @brief    ����
#define ZRCS_LEAVE(cs) LeaveCriticalSection(cs)

/// * @brief    ��������Ϊ��������ϵͳ
#define MUTEX CRITICAL_SECTION
/// * @brief    ����
#define MUTEX_LOCK(mutex) EnterCriticalSection(&mutex)
/// * @brief    ����
#define MUTEX_UNLOCK(mutex) LeaveCriticalSection(&mutex)
/// * @brief    ��������ʼ��
#define MUTEX_INIT(mutex) InitializeCriticalSection(&mutex)
/// * @brief    ����������
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(&mutex)

//�ź�����Դ
/// * @brief    �ź���
typedef HANDLE SEMA;
/// * @brief    �ȴ��ź���һ��ʱ��
#define SEMA_WAIT_TIME(sema, delay) WaitForSingleObject(sema, delay)
/// * @brief    һֱ�����ؽ��еȴ��ź���
#define SEMA_WAIT(sema) WaitForSingleObject(sema, INFINITE)
/// * @brief    �ͷ��ź���
#define SEMA_POST(sema) ReleaseSemaphore(sema, 1, NULL)
/// * @brief    ���Ի�ȡһ���ź���
#define SEMA_TRYWAIT(sema) WaitForSingleObject(sema, 0)
/// * @brief    �����ź���
#define SEMA_DESTROY(sema) CloseHandle(sema)
/// * @brief    ��ʼ���ź����� �����Ϊ���ź��������ֵ����ʼ�ź�������
#define SEMA_INIT(sema, initCount, maxCount) sema = CreateSemaphore(NULL, initCount, maxCount, NULL)
/// * @brief    ��ʼһ���������Ƶ��ź��������ڶ���̽���
#define SEMA_INIT_NAME(sema, initCount, maxCount, semaName) sema = CreateSemaphore(NULL, initCount, maxCount, semaName)
/// * @brief    �ź����ȴ���ʱ
#define SEMA_WAIT_TIMEOUT WAIT_TIMEOUT
/// * @brief    �ȴ����ź���
#define SEMA_WAIT_OK WAIT_OBJECT_0



#elif defined(_VXWORKS)

//�ٽ�����Դ
typedef SEM_ID ZRCRITICALSECTION;
#define INVALID_CRITSECT NULL
inline ZRCRITICALSECTION ZRCreateCriticalSection(int spinCount = 0)
{
	ZRCRITICALSECTION cs = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
	if (cs == NULL)
	{
		perror("vxworks create MUTUAL EXCLUSION SEMAPHORE failed\n");
	}
	return cs;
}
inline void ZRDeleteCriticalSection(ZRCRITICALSECTION & cs)
{
	semDelete(cs);
	cs = INVALID_CRITSECT;
}

#define ZRCS_ENTER(cs) semTake(cs, WAIT_FOREVER)
#define ZRCS_LEAVE(cs) semGive(cs)

#define MUTEX SEM_ID
#define MUTEX_LOCK(mutex) semTake(mutex, WAIT_FOREVER)
#define MUTEX_UNLOCK(mutex) semGive(mutex)
#define MUTEX_INIT(mutex) mutex = semBCreate(SEM_Q_FIFO,SEM_FULL)
#define MUTEX_DESTROY(mutex) semDelete(mutex)
//�ź�����Դ
#define SEMA SEM_ID
#define SEMA_WAIT_TIME(sema,delay) semTake(sema, delay)
#define SEMA_WAIT(sema) semTake(sema, WAIT_FOREVER)
#define SEMA_POST(sema) semGive(sema)
#define SEMA_DESTROY(sema) semDelete(sema)
#define SEMA_INIT(sema, initCount, maxCount) sema = semCCreate(SEM_Q_FIFO,initCount)
#define SEMA_WAIT_TIMEOUT ERROR
//�߳���Դ
#define THREADID int
#define SOCKET int
#define closesocket(s_) close(s_)
#define SOCKET_ERROR -1

#elif defined(__linux)



//�ٽ�����Դ
typedef pthread_mutex_t* ZRCRITICALSECTION;
#define INVALID_CRITSECT NULL
// !!!!!!!!!!!!!!!!
// *ע��*
// ��Linux�µ�mutex�������������ȴ�ʱ��
// ���Ըú����Ĳ�����������
inline ZRCRITICALSECTION ZRCreateCriticalSection()
{
	ZRCRITICALSECTION cs = new pthread_mutex_t;
	int ret;
	assert(cs != INVALID_CRITSECT);
	

	pthread_mutexattr_t mutexattr; 
	pthread_mutexattr_init(&mutexattr); 
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP); 
	pthread_mutex_init(cs, &mutexattr); 
	pthread_mutexattr_destroy(&mutexattr);

	
	return cs;
}
inline void ZRDeleteCriticalSection(ZRCRITICALSECTION & cs)
{
	if (cs != INVALID_CRITSECT)
	{
		pthread_mutex_destroy(cs);
		delete cs;
		cs = 0;
	}
}

#define ZRCS_ENTER(cs) pthread_mutex_lock(cs)
#define ZRCS_LEAVE(cs) pthread_mutex_unlock(cs)




#define MUTEX pthread_mutex_t
#define SEMA sem_t
#define MUTEX_LOCK(mutex) pthread_mutex_lock(&mutex)
#define MUTEX_UNLOCK(mutex) pthread_mutex_unlock(&mutex)
#define MUTEX_INIT(mutex) pthread_mutex_init(&mutex,NULL)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(&mutex)


inline int sema_wait_time_(sem_t* sema, unsigned int delay)
{
	struct timespec ts;
	if (::clock_gettime(CLOCK_REALTIME, &ts) != 0)
	{
		return -1;
	}

	ts.tv_sec += delay / 1000;
	ts.tv_nsec = (delay % 1000) * 1000000;
	ts.tv_sec += ts.tv_nsec / 1000000000;
	ts.tv_nsec %= 1000000000;

	return sem_timedwait(sema, &ts) == 0 ? 0 : ETIMEDOUT;
}

#define SEMA_WAIT_TIME(sema,delay) sema_wait_time_(&sema,delay)
#define SEMA_WAIT(sema) sem_wait(&sema)
#define SEMA_POST(sema) sem_post(&sema)
#define SEMA_TRYWAIT(sema) sem_trywait(&sema)
#define SEMA_DESTROY(sema) sem_destroy(&sema)
#define SEMA_INIT(sema, initCount, maxCount) sem_init(&sema,0,initCount)
#define SEMA_INIT_NAME(sema, initCount, maxCount, semaName) sema = sem_open(semaName, O_CREAT, 0, initCount)
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT ETIMEDOUT
#define SEMA_WAIT_TIMEOUT ETIMEDOUT
#define SEMA_WAIT_OK 0



// ����Socket��Դ
typedef int SOCKET;

typedef struct _FILETIME
{
	unsigned long dwLowDateTime;
	unsigned long dwHighDateTime;
}FILETIME;
// @add end

#endif //WIN32


#endif //_ZRMULTITHREADUTILITY_