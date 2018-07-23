#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#include "Signal.h"

#define SIGNAL_STD_TEST                 0
#define SIGNAL_EXT_TEST                 0
#define SIGNAL_EXT_WITHPARAM_TEST       1

#if SIGNAL_STD_TEST
static VOID SignalHadlerFunc(INT32 iSigNum)
{
    printf("Catch signal %d\r\n", iSigNum);

    return;
}
#endif

#if SIGNAL_EXT_TEST
static VOID CatchSignal(INT32 iSigNum)
{
    switch(iSigNum)
    {
        case SIGINT:
            //SIGINT默认行为是退出进程
            printf("SIGINT signal\n");
            exit(0);
            break;
        case SIGALRM:
            //SIGALRM默认行为是退出进程
            printf("SIGALRM signal\n");
            break;
        case SIGKILL:
            printf("SIGKILL signal\n");
            break;
    }

    return;
}

#endif

#if SIGNAL_EXT_WITHPARAM_TEST
#define TEST_STR            "123456qwert"
#define TEST_LEN            100

VOID CatchActionSignal(INT32 iSigNum, siginfo_t *pstResData, VOID *pvUnkonw)
{
    /* 整数测试 */
#if 1
    printf("iSigNum = %d\n", iSigNum);
    printf("return data :%d\n", pstResData->si_value.sival_int);
    printf("second return data:%d\n", pstResData->si_int);
#else
    /* 字符串测试 */
    printf("Hi.\r\n");
    printf("iSigNum = %d\n", iSigNum);
    printf("return data :%s\n", pstResData->si_value.sival_ptr);
#endif
    
    return;
}

#endif

int main()
{
#if SIGNAL_STD_TEST
    UINT32 iCnt = 0;
    pid_t iPid = -1;
    
    /* 注册SIGINT信号处理 */
    SignalRegister(SIGINT, SignalHadlerFunc);

    /* 注册SIGALRM信号处理 */
    SignalRegister(SIGALRM, SignalHadlerFunc);

    /* 获取进程ID号 */
    iPid = getpid();
    printf("Current PID:%d\r\n", iPid);

    for (;;)
    {
        iCnt++;

        /* 测试信号发送 */
        #if 0
        #if 0
        SignalKillSend(iPid, SIGINT);
        #else
        SignalRaiseSend(SIGINT);
        #endif
        #endif

        /* 测试定时器SIGALRM信号 */
        /* 注意：当alarm=5，而sleep=1秒时，alarm会失效，所以编程时要考虑清楚这一点 */
        SignalAlarmSend(2);
        //alarm(1);
        
        sleep(3);

        /* 30秒后自动退出 */
        if (iCnt > 30)
        {
            break;
        }
    }
#endif

#if SIGNAL_EXT_TEST
    INT32 i = 0;
    
    SignalActionRegister(SIGINT, CatchSignal);
    SignalActionRegister(SIGALRM, CatchSignal);
    SignalActionRegister(SIGKILL, CatchSignal);
    
    while(1)
    {
        printf("Hello good  %d\n",i++);
        sleep(1);
    }

#endif

#if SIGNAL_EXT_WITHPARAM_TEST
    pid_t iPid=0;
    INT32 iValue = 0;
    INT32 iRet = 0;
    INT32 iStatus = 0;
#if 0
    CHAR acTest[TEST_LEN];
#endif
    
    iPid = fork();
    if (iPid == -1)
    {
        printf("fork() failed! error message:%s\n", strerror(errno));
        return -1;
    }
    
    if (iPid==0)
    {
        printf("I am child.\n");
        //等待父进程执行完信号安装
        sleep(2);

#if 1
        iValue = 6688;

        iRet = SignalActionWithParamSend(getppid(), SIGINT, iValue);
        if (iRet < 0)
        {
            printf("sigqueue() failed ! error message:%s\n",strerror(errno));
            exit(0);
        }
#endif
        
        printf("子进程信号发送成功！\n");
        exit(0);
    }
    
    if (iPid>0)
    {
        printf("I am father.\n");
        iRet = SignalActionWithParamRegister(SIGINT, CatchActionSignal);
        if (iRet < 0)
        {
            printf("sigaction() failed! \n");
            return -1;
        }

        //因为传递的是4字节的内容，所以不能跨进程、父子进程也不能传递字符串
#if 0
        memcpy(acTest, TEST_STR, strlen(TEST_STR) + 1);

        iRet = SignalActionWithParamSend(getpid(), SIGINT, (INT32)&acTest);
        if (iRet < 0)
        {
            printf("sigqueue() failed ! error message:%s\n",strerror(errno));
            exit(0);
        }
#endif
        
        iStatus = 0;
        wait(&iStatus);
        
        printf("game over.\n");
    }

#endif

    return 0;
}


