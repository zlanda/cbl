/*******************************************************************************
*   文  件 名：Signal.c
*   功     能：封装信号功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/

#include "Process.h"
#include "Signal.h"

/*******************************************************************************
*   函 数   名：SignalRegister
*   功     能：注册一个信号的处理句柄
*   输入参数：iSigNum:信号值
*             fSigProcessHandler:信号对应的处理函数
*   输出参数：无
*   返 回 值：-1:创建管道失败
*             >0:创建成功
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
fSignalHandler SignalRegister(INT32 iSigNum, fSignalHandler fSigProcessHandler)
{
	return signal(iSigNum, fSigProcessHandler);
}

/*******************************************************************************
*   函 数   名：SignalKillSend
*   功     能：用于向任何进程组或进程发送信号
*   输入参数：iPid:可以选择以下四种：
*               iPid>0:iPid是信号欲送往的进程的标识
*                   将信号iSigNum发给iPid进程
*               iPid=0:信号将送往所有与调用kill()的那个进程属于同一个使用组的进程
*                   将信号iSigNum发给组进程
*               iPid=-1:信号将送往所有调用进程有权给其发送信号的进程，除了进程1（init）
*                   将信号iSigNum发送给所有进程，调用者进程有权限发送的每一个进程（除了1号进程之外，还有它自身）
*               iPid<-1:信号将送往以-iPid为组标识的进程
*                   将信号iSigNum发送给进程组iPid（绝对值）的每一个进程
*             iSigNum:准备发送的信号
*   输出参数：无
*   返 回 值：成功执行返回0
*             失败返回-1，并设置errno
*   说     明：kill既可以向自身发送信号，也可以向其他进程发送信号。
*             raise函数向进程自身发送信号。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 SignalKillSend(pid_t iPid, INT32 iSigNum)
{
	return kill(iPid, iSigNum);
}

/*******************************************************************************
*   函 数   名：SignalRaiseSend
*   功     能：进程向自己发送信号
*   输入参数：iSigNum:准备发送的信号
*   输出参数：无
*   返 回 值：成功执行返回0
*             失败时返回非0值
*   说     明：kill既可以向自身发送信号，也可以向其他进程发送信号。
*             raise函数向进程自身发送信号。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 SignalRaiseSend(INT32 iSigNum)
{
	return raise(iSigNum);
}

/*******************************************************************************
*   函 数   名：SignalAbortSend
*   功     能：异常终止一个进程
*   输入参数：无
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
VOID SignalAbortSend()
{
    abort();
    
	return;
}

/*******************************************************************************
*   函 数   名：SignalAlarmSend
*   功     能：设置信号SIGALRM在经过参数uiSeconds指定的秒数后传送给目前的进程。
*   输入参数：uiSeconds：如果参数seconds为0，则之前设置的闹钟会被取消，并将剩下的时间返回
*   输出参数：无
*   返 回 值：返回之前闹钟的剩余秒数，如果之前未设闹钟则返回0。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
UINT32 SignalAlarmSend(UINT32 uiSeconds)
{
    return alarm(uiSeconds);
}

/*******************************************************************************
*   函 数   名：SignalActionRegister
*   功     能：注册信号处理句柄,依参数iSigNum指定的信号编号来设置该信号的处理函数。
*             参数iSigNum可以指定SIGKILL和SIGSTOP以外的所有信号。
*   输入参数：iSigNum：具体信号，它可以是除了SIGKILL和SIGSTOP之外的任何有效信号值。
*             fSigProcessHanlder：信号处理句柄
*   输出参数：无
*   返 回 值：失败返回负值
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 SignalActionRegister(INT32 iSigNum, fSignalHandler fSigProcessHanlder)
{
    struct sigaction stSigAction;   /* 将要安装的iSigNum定义信号的新行动。*/
    struct sigaction stOAction;     /* 用来保存iSigNum定义信号的过去的行动。*/

    /*传入回调函数,这个成员指出了和iSigNum关联的行动，设置为SIG_DFL表示默认行动，
      设置为SIG_IGN表示忽略这个信号，或者设置为处理函数的指针。这个函数接收iSigNum
      作为它的唯一参数。*/
    stSigAction.sa_handler = fSigProcessHanlder;

    /* 将act的属性sa_mask设置一个初始值 */
    sigemptyset(&stSigAction.sa_mask);
    /* 给出了一个可以改变信号行为的标志集合。它由以下零个或多个值的按位或形成：
            SA_NOCLDSTOP
            SA_NOCLDWAIT
            SA_NODEFER
            SA_ONSTACK
            SA_RESETHAND
            SA_RESTART
            SA_SIGINFO
      如果sa_flags被指定为SA_SIGINFO，那么sa_sigacton代替sa_handler表示iSigNum信号处理函数。
    */
    stSigAction.sa_flags = 0;

    return sigaction(iSigNum, &stSigAction, &stOAction);
}

/*******************************************************************************
*   函 数   名：SignalActionWithParamRegister
*   功     能：注册信号处理句柄,依参数iSigNum指定的信号编号来设置该信号的处理函数。
*             参数iSigNum可以指定SIGKILL和SIGSTOP以外的所有信号。
*             可给信号处理函数传递参数
*   输入参数：iSigNum：具体信号，它可以是除了SIGKILL和SIGSTOP之外的任何有效信号值。
*             fSigProcessHanlder：信号处理句柄
*   输出参数：无
*   返 回 值：失败返回负值
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 SignalActionWithParamRegister(INT32 iSigNum, fSignalActionHandler fSigProcessHanlder)
{
    struct sigaction stSigAction;   /* 将要安装的iSigNum定义信号的新行动。*/
    struct sigaction stOAction;     /* 用来保存iSigNum定义信号的过去的行动。*/

    stSigAction.sa_sigaction = fSigProcessHanlder;

    /* 将act的属性sa_mask设置一个初始值 */
    sigemptyset(&stSigAction.sa_mask);
    stSigAction.sa_flags = SA_SIGINFO;

    return sigaction(iSigNum, &stSigAction, &stOAction);
}

/*******************************************************************************
*   函 数   名：SignalActionWithParamRegister
*   功     能：注册信号处理句柄,依参数iSigNum指定的信号编号来设置该信号的处理函数。
*             参数iSigNum可以指定SIGKILL和SIGSTOP以外的所有信号。
*             可给信号处理函数传递参数
*             只能向一个进程发送信号，而不能发送信号给一个进程组。
*   输入参数：iPid：向指定进程发送信号
*             iSigNum：发送指定信号
*             iDataValue：发送信号的参数，注意sigval是个联合体，指针是4字节的，因此最终也是4字节
*   输出参数：无
*   返 回 值：失败返回负值
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 SignalActionWithParamSend(pid_t iPid, INT32 iSigNum, INT32 iDataValue)
{
    union sigval unSigValue;

    unSigValue.sival_int = iDataValue;

    return sigqueue(iPid, iSigNum, unSigValue);
}

