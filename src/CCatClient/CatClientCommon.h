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


#define CAT_SUCCESS "0"
#define CAT_ERROR "-1"
#define CAT_FAIL "FAIL"

// ¼ì²éÉêÇë³öÀ´µÄÄÚ´æ£¬Èç¹ûÉêÇë³öÀ´µÄÎªNULL£¬Ôò´ú±íµ±Ç°³ÌÐòÄÚ´æÒÑÂú
// Ä¿Ç°ÏÈ¼òµ¥ÊµÏÖ£¬Ö»ÊÇ´òÓ¡£¬È»ºóÍË³ö
// Î´À´ÐèÒªÔÚ¼ì²âµ½Òì³£Ê±£¬Ö±½Ó³¢ÊÔ·¢ËÍÒ»ÌõÄÚ´æÒÑÂúµÄÏûÏ¢µ½·þÎñÆ÷
// ×¢Òâ£¬ÕâÌõÏûÏ¢Ò»¶¨ÒªÊÂÏÈÉêÇëºÃÄÚ´æ£¬×îºÃÖ±½Óµ÷ÓÃµ±Ç°¿ÉÓÃµÄsocket·¢ËÍ
void catChecktPtr(void *);

static inline unsigned long long catTrimToHour(unsigned long long timeMs)
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