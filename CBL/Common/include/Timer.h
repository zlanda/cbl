/*******************************************************************************
*   文  件 名：Timer.h
*   功     能：时间及定时器对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_COMMON_TIMER_H_
#define  _CBL_COMMON_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <time.h>
#include <sys/time.h>

/* 内部头文件 */
#include "Types.h"

typedef VOID (*fTimerProcessHandler)(union sigval unSigVal);

/* 时间及定时器对外接口 */
time_t Time(time_t *pstSeconds);

struct tm *LocalTime(const time_t *pstSeconds);

CHAR *AscTime(const struct tm *pstTime);

clock_t Clock(VOID);

CHAR *CTime(const time_t *pstTime);

struct tm *GmTime(const time_t *pstTime);

time_t MkTime(struct tm *pstTime);

size_t StrFTime(CHAR *pcStr, size_t iMaxSize, const CHAR *pcFormat, const struct tm *pstTime);

INT32 GetTimeOfDay(struct timeval *pstTimeVal, struct timezone *pstTimeZone);

INT32 SetTimeOfDay(struct timeval *pstTimeVal, struct timezone *pstTimeZone);

VOID CreateSelectTimer(INT32 iSeconds, INT32 iMesconds);

VOID CreateAlarmTimer(INT32 iSeconds, fSignalActionHandler fSigTimeoutHandler);

VOID CreateAlarmStdTimer(INT32 iSeconds, fSignalHandler fSigTimeoutHandler);

VOID CreateTimer(INT32 iStartDelay, INT32 iTimerDelay, fTimerProcessHandler fTimerHandler);

INT32 CreateSignalTimer(INT32 iStartDelay, INT32 iTimerDelay, timer_t *pstTimerId, fSignalHandler fTimerHandler);

VOID SetTimer(timer_t *pstTimerId, INT32 iStartInterval, INT32 iTimerInterval);

VOID CancleTimer(timer_t *pstTimerId);

INT32 DeleteTimer(timer_t *pstTimerId);

#ifdef __cplusplus
}
#endif

#endif

