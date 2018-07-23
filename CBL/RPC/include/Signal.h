/*******************************************************************************
*   文  件 名：Signal.h
*   功     能：信号对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_SIGNAL_H_
#define  _CBL_RPC_SIGNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

/* 内部头文件 */
#include "Types.h"

/* 定义传递给信号处理的参数是整数 */
#define SIG_PARAM_INT                       1

/* 定义传递给信号处理的参数是字符串或其他类型 */
#define SIG_PARAM_NON_INT                   0

/* 定义标准信号处理句柄 */
typedef VOID (*fSignalHandler)(INT32);

/* 定义传递参数的信号处理句柄 */
/* siginfo_t定义 */
#if 0
    typedef union sigval
    {
        int sival_int;
        void *sival_ptr;
    } sigval_t;
 
    siginfo_t 
    {
        int      si_signo;  /* Signal number */
        int      si_errno;  /* An errno value */
        int      si_code;   /* Signal code */
        pid_t    si_pid;    /* Sending process ID */
        uid_t    si_uid;    /* Real user ID of sending process */
        int      si_status; /* Exit value or signal */
        clock_t  si_utime;  /* User time consumed */
        clock_t  si_stime;  /* System time consumed */
        sigval_t si_value;  /* Signal value */ 
        int      si_int;    /* POSIX.1b signal */
        void *   si_ptr;    /* POSIX.1b signal */
        void *   si_addr;   /* Memory location which caused fault */
        int      si_band;   /* Band event */
        int      si_fd;     /* File descriptor */
    }
#endif
typedef VOID (*fSignalActionHandler)(INT32, siginfo_t *, VOID *);

/* 定义信号对外接口 */
/* 注册信号处理句柄 */
fSignalHandler SignalRegister(INT32 iSigNum, fSignalHandler fSigProcessHandler);

/* 发送信号 */
INT32 SignalKillSend(pid_t iPid, INT32 iSigNum);

/* 给进程自身发送信号 */
INT32 SignalRaiseSend(INT32 iSigNum);

/* 给进程发送终止信号 */
VOID SignalAbortSend();

/* 每个指定时间发送alarm信号 */
UINT32 SignalAlarmSend(UINT32 uiSeconds);

/* 注册信号处理句柄 */
INT32 SignalActionRegister(INT32 iSigNum, fSignalHandler fSigProcessHanlder);

/* 注册信号处理句柄，能传递参数 */
INT32 SignalActionWithParamRegister(INT32 iSigNum, fSignalActionHandler fSigProcessHanlder);

/* 发送信号，能传递参数 */
INT32 SignalActionWithParamSend(pid_t iPid, INT32 iSigNum, INT32 iDataValue);

#ifdef __cplusplus
}
#endif

#endif

