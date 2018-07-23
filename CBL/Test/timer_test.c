#include <stdio.h>
#include <stdlib.h>
#include "Signal.h"
#include "Timer.h"

#define TIME_TEST                           0
#define TIMER_SELECT_TEST                   0
#define TIMER_ALARM_TEST                    0
#define TIMER_ALARM_STD_TEST				1

#if TIME_TEST
#define FORMAT_TIME_LEN                     80
#endif

#if TIMER_ALARM_TEST
VOID TimerProcessTimeout(INT32 iSigNum, siginfo_t *pstResData, VOID *pvUnkonw)
{
    INT32 iTimeout = 0;
    static INT32 iCnt = 0;
    
    printf("iSigNum = %d\n", iSigNum);
    printf("return data :%d\n", pstResData->si_value.sival_int);

    iTimeout = pstResData->si_value.sival_int;

    sleep(iTimeout);
    iCnt++;
    if (iCnt >= 5)
    {
        return;
    }

    /* 重新设定alarm的定时器 */
    SignalActionWithParamSend(getpid(), SIGALRM, iTimeout);
    
    return;
}
#endif

#if TIMER_ALARM_STD_TEST
VOID TimerProcessTimeout(INT32 iSigNum)
{
	INT32 iTimeout = 2;
	
    printf("Recv SigNum:%d\n", iSigNum);

    /* 重新设定alarm的定时器 */
    SignalAlarmSend(iTimeout);
    
    return;
}

#endif

int main()
{
#if TIME_TEST
    time_t stSeconds;
    struct tm *pstTm;
    CHAR *pcTimeStr;
    CHAR acTimeBuffer[FORMAT_TIME_LEN];
    struct timeval stCurTime;
	struct timezone stCurZone;

    /* 获取系统日历时间 */
    stSeconds = Time(CBL_NULL);
    
    //printf("自 1970-01-01 起的小时数 = %ld\n", stSeconds/3600);
    printf("Time = %ld\n", stSeconds/3600);

    /* 将日历时间转换成本地时间 */
    pstTm = LocalTime(&stSeconds);

    /* 转换成字符串 */
    pcTimeStr = AscTime(pstTm);
    printf("LocalTime:%s\r\n", pcTimeStr);

    /* 格式化一下 */
    StrFTime(acTimeBuffer, FORMAT_TIME_LEN, "%x - %I:%M%p", pstTm);
    printf("Format Time:|%s|\n", acTimeBuffer);

    /* 测试一下gettimeofday */
    GetTimeOfDay(&stCurTime, &stCurZone);
    
    printf("current time of sec is %ld, usec is %ld\n", stCurTime.tv_sec, stCurTime.tv_usec);
    printf("current zone is %d, dsttime is %d\n", stCurZone.tz_minuteswest, stCurZone.tz_dsttime);

    stCurTime.tv_sec +=60;
    SetTimeOfDay(&stCurTime, &stCurZone);
    printf("Set...\r\n");
    printf("current time of sec is %ld, usec is %ld\n", stCurTime.tv_sec, stCurTime.tv_usec);
    printf("current zone is %d, dsttime is %d\n", stCurZone.tz_minuteswest, stCurZone.tz_dsttime);
    
#endif

#if TIMER_SELECT_TEST
    INT32 i = 0;

    /* 因为是以selcet为基础的，所以程序会阻塞在select调用处直到超时 */
    for (i = 0; i < 3; i++)
    {
        CreateSelectTimer(10, 0);
    }
    
#endif

#if TIMER_ALARM_TEST
    INT32 iTimeout = 2;
    
    CreateAlarmTimer(iTimeout, TimerProcessTimeout);
#endif

#if TIMER_ALARM_STD_TEST
	INT32 iTimeout = 2;
	UINT32 iIndex = 0;
    
    CreateAlarmStdTimer(iTimeout, TimerProcessTimeout);

    while (1)
    {
		sleep(1);
		iIndex++;
		if (iIndex > 5)
		{
			break;
		}
    }
#endif

    return 0;
}


