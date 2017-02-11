#ifndef CATCLIENTCOMMON_H
#define CATCLIENTCOMMON_H

#ifdef WIN32
#define inline __inline
#endif

// 检查申请出来的内存，如果申请出来的为NULL，则代表当前程序内存已满
// 目前先简单实现，只是打印，然后退出
// 未来需要在检测到异常时，直接尝试发送一条内存已满的消息到服务器
// 注意，这条消息一定要事先申请好内存，最好直接调用当前可用的socket发送
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