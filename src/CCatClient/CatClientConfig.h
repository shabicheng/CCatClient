
#ifndef CATCLIENT_H
#define CATCLIENT_H

#ifdef WIN32

#ifdef  CCATCLIENT_EXPORTS
#define  CATCLIENT_EXPORT __declspec(dllexport)
#else
#define  CATCLIENT_EXPORT __declspec(dllimport)
#endif

#else

#define CATCLIENT_EXPORT

#endif

#endif //CATCLIENT_H
