// /*************************************************************************************************
// @Assembly         
// @Author           ZRZC
// @Created          11-17-2015
//
// @Last Modified By ZRZC
// @Last Modified On 11-17-2015
// 
// @copyright        Copyright (c) ZhenRong. All rights reserved.
// @summary          多线程操作的一些基本资源封装，信号量、互斥锁、临界区
// **************************************************************************************************/
#ifndef _ZRMULTITHREADUTILITY_
#define _ZRMULTITHREADUTILITY_

#include "ZRLibraryGlobal.h"


//以下为各种和操作系统资源相关的工具宏定义
#ifdef WIN32
/// * @brief    /临界区资源
typedef LPCRITICAL_SECTION ZRCRITICALSECTION;
#define INVALID_CRITSECT NULL
/// *************************************************************************************************
/// * @fn       ZRCreateCriticalSection
/// * @brief    创建互斥锁.
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
static inline ZRCRITICALSECTION ZRCreateCriticalSection()
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
/// * @brief    删除互斥锁
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
static inline void ZRDeleteCriticalSection(ZRCRITICALSECTION cs)
{
	if (cs != INVALID_CRITSECT)
	{
		DeleteCriticalSection(cs);
		free(cs);
		cs = 0;
	}
}

/// * @brief    加锁
#define ZRCS_ENTER(cs) EnterCriticalSection(cs)
/// * @brief    解锁
#define ZRCS_LEAVE(cs) LeaveCriticalSection(cs)

/// * @brief    互斥锁，为兼容已有系统
#define MUTEX CRITICAL_SECTION
/// * @brief    加锁
#define MUTEX_LOCK(mutex) EnterCriticalSection(&mutex)
/// * @brief    解锁
#define MUTEX_UNLOCK(mutex) LeaveCriticalSection(&mutex)
/// * @brief    互斥锁初始化
#define MUTEX_INIT(mutex) InitializeCriticalSection(&mutex)
/// * @brief    互斥锁销毁
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(&mutex)

//信号量资源
/// * @brief    信号量
typedef HANDLE SEMA;
/// * @brief    等待信号量一定时间
#define SEMA_WAIT_TIME(sema, delay) WaitForSingleObject(sema, delay)
/// * @brief    一直阻塞地进行等待信号量
#define SEMA_WAIT(sema) WaitForSingleObject(sema, INFINITE)
/// * @brief    释放信号量
#define SEMA_POST(sema) ReleaseSemaphore(sema, 1, NULL)
/// * @brief    尝试获取一个信号量
#define SEMA_TRYWAIT(sema) WaitForSingleObject(sema, 0)
/// * @brief    销毁信号量
#define SEMA_DESTROY(sema) CloseHandle(sema)
/// * @brief    初始化信号量， 输入的为：信号量的最大值，初始信号量个数
#define SEMA_INIT(sema, initCount, maxCount) sema = CreateSemaphore(NULL, initCount, maxCount, NULL)
/// * @brief    初始一个带有名称的信号量，用于多进程交互
#define SEMA_INIT_NAME(sema, initCount, maxCount, semaName) sema = CreateSemaphore(NULL, initCount, maxCount, semaName)
/// * @brief    信号量等待超时
#define SEMA_WAIT_TIMEOUT WAIT_TIMEOUT
/// * @brief    等待到信号量
#define SEMA_WAIT_OK WAIT_OBJECT_0



#elif defined(_VXWORKS)

//临界区资源
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
//信号量资源
#define SEMA SEM_ID
#define SEMA_WAIT_TIME(sema,delay) semTake(sema, delay)
#define SEMA_WAIT(sema) semTake(sema, WAIT_FOREVER)
#define SEMA_POST(sema) semGive(sema)
#define SEMA_DESTROY(sema) semDelete(sema)
#define SEMA_INIT(sema, initCount, maxCount) sema = semCCreate(SEM_Q_FIFO,initCount)
#define SEMA_WAIT_TIMEOUT ERROR
//线程资源
#define THREADID int
#define SOCKET int
#define closesocket(s_) close(s_)
#define SOCKET_ERROR -1

#elif defined(__linux)



//临界区资源
typedef pthread_mutex_t* ZRCRITICALSECTION;
#define INVALID_CRITSECT NULL
// !!!!!!!!!!!!!!!!
// *注意*
// 在Linux下的mutex不能设置自旋等待时间
// 所以该函数的参数将被忽略
inline ZRCRITICALSECTION ZRCreateCriticalSection()
{
	ZRCRITICALSECTION cs = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	int ret;
	assert(cs != INVALID_CRITSECT);
	

	pthread_mutexattr_t mutexattr; 
	pthread_mutexattr_init(&mutexattr); 
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP); 
	pthread_mutex_init(cs, &mutexattr); 
	pthread_mutexattr_destroy(&mutexattr);

	
	return cs;
}
static inline void ZRDeleteCriticalSection(ZRCRITICALSECTION cs)
{
	if (cs != INVALID_CRITSECT)
	{
		pthread_mutex_destroy(cs);
		free(cs);
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


static inline int sema_wait_time_(sem_t* sema, unsigned int delay)
{
    struct timespec ts;
    
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tv.tv_usec += (delay % 1000) * 1000;
    tv.tv_sec += delay / 1000;
    if (tv.tv_usec > 1000000)
    {
        tv.tv_usec -= 1000000;
        ++tv.tv_sec;
    }


	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = tv.tv_usec * 1000;

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



// 网络Socket资源
//typedef int SOCKET;

typedef struct _FILETIME
{
	unsigned long dwLowDateTime;
	unsigned long dwHighDateTime;
}FILETIME;
// @add end

#endif //WIN32


#endif //_ZRMULTITHREADUTILITY_