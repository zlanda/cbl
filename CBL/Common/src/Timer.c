/*******************************************************************************
*   文  件 名：Timer.c
*   功     能：封装时间及定时器功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/

#include "Signal.h"
#include "Timer.h"

/*******************************************************************************
*   函 数   名：Time
*   功     能：获取当前的系统时间，返回的结果是一个time_t类型，其实就是一个大整数，其值
*             表示从CUT(Coordinated Universal Time)时间1970/7/1 00:00:00（称为UNIX系统的Epoch时间）
*             到当前时刻的秒数。然后调用localtime将time_t所表示的CUT时间转换为本地时间（我们是+8区，
*             比CUT多8个小时）并转成struct tm类型，该类型的各数据成员分别表示年月日时分秒。
*             以秒为单位。如果seconds不为空，则返回值也存储在变量pstSeconds中
*   输入参数：无
*   输出参数：这是指向类型为time_t的对象的指针，用来存储seconds的值。
*   返 回 值：以time_t对象返回当前日历时间。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
time_t Time(time_t *pstSeconds)
{
    return time(pstSeconds);
}

/*******************************************************************************
*   函 数   名：LocalTime
*   功     能：timer 的值被分解为tm结构，并用本地时区表示。
*                struct tm {
*                   int tm_sec;         // 秒，范围从 0 到 59
*                   int tm_min;         // 分，范围从 0 到 59
*                   int tm_hour;        // 小时，范围从 0 到 23
*                   int tm_mday;        // 一月中的第几天，范围从 1 到 31
*                   int tm_mon;         // 月份，范围从 0 到 11
*                   int tm_year;        // 自 1900 起的年数
*                   int tm_wday;        // 一周中的第几天，范围从 0 到 6
*                   int tm_yday;        // 一年中的第几天，范围从 0 到 365
*                   int tm_isdst;       // 夏令时
*                };
*   输入参数：指向表示日历时间的 time_t 值的指针。
*   输出参数：无
*   返 回 值：返回tm结构
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
struct tm *LocalTime(const time_t *pstSeconds)
{
    return localtime(pstSeconds);
}

/*******************************************************************************
*   函 数   名：LocalTime
*   功     能：将struct tm结构表示的日期和时间转换成字符串
*                struct tm {
*                   int tm_sec;         // 秒，范围从 0 到 59
*                   int tm_min;         // 分，范围从 0 到 59
*                   int tm_hour;        // 小时，范围从 0 到 23
*                   int tm_mday;        // 一月中的第几天，范围从 1 到 31
*                   int tm_mon;         // 月份，范围从 0 到 11
*                   int tm_year;        // 自 1900 起的年数
*                   int tm_wday;        // 一周中的第几天，范围从 0 到 6
*                   int tm_yday;        // 一年中的第几天，范围从 0 到 365
*                   int tm_isdst;       // 夏令时
*                };
*   输入参数：指向struct tm结构的日期和时间
*   输出参数：无
*   返 回 值：该函数返回一个C字符串，包含了可读格式的日期和时间信息 Www Mmm dd hh:mm:ss yyyy，
*             其中，Www 表示星期几，Mmm 是以字母表示的月份，dd 表示一月中的第几天，
*             hh:mm:ss 表示时间，yyyy 表示年份。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
CHAR *AscTime(const struct tm *pstTime)
{
    return asctime(pstTime);
}

/*******************************************************************************
*   函 数   名：Clock
*   功     能：返回自程序执行起（一般为程序的开头），处理器始终所使用的时间。为了
*             获取CPU所使用的秒数，你需要除以CLOCKS_PER_SEC，在32位系统中，CLOCKS_PER_SEC
*             等于1000000，该函数大约每72分钟会返回相同的值。
*   输入参数：无
*   输出参数：无
*   返 回 值：该函数返回自程序启动起，处理器时钟所使用的时间。如果失败，则返回 -1 值。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
clock_t Clock(VOID)
{
    return clock();
}

/*******************************************************************************
*   函 数   名：CTime
*   功     能：返回一个表示当地时间的字符串，当地时间是基于参数pstTime。返回的字符串
*             格式如下：Www Mmm dd hh:mm:ss yyyy 其中，Www 表示星期几，
*             Mmm 是以字母表示的月份，dd 表示一月中的第几天，hh:mm:ss 表示时间，
*             yyyy 表示年份。
*   输入参数：pstTime:这是指向 time_t 对象的指针，该对象包含了一个日历时间。
*   输出参数：无
*   返 回 值：该函数返回一个 C 字符串，该字符串包含了可读格式的日期和时间信息。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
CHAR *CTime(const time_t *pstTime)
{
    return ctime(pstTime);
}

/*******************************************************************************
*   函 数   名：GmTime
*   功     能：使用pstTime的值来填充tm结构，并用GMT表示
*   输入参数：pstTime:这是指向表示日历时间的 time_t 值的指针。
*   输出参数：无
*   返 回 值：该函数返回指向 tm 结构的指针，该结构带有被填充的时间信息。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
struct tm *GmTime(const time_t *pstTime)
{
    return gmtime(pstTime);
}

/*******************************************************************************
*   函 数   名：MkTime
*   功     能：将pstTime所指向的结构转换为一个依据本地时区的time_t值。
*   输入参数：pstTime:指向struct tm结构的指针
*   输出参数：无
*   返 回 值：该函数返回一个 time_t 值，该值对应于以参数传递的日历时间。如果发生错误，则返回 -1 值。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
time_t MkTime(struct tm *pstTime)
{
    return mktime(pstTime);
}

/*******************************************************************************
*   函 数   名：StrFTime
*   功     能：根据pcFormat中定义的格式化规则，格式化结构pstTime表示的时间，并把
*             它存储在pcStr中。
*   输入参数：pcStr:指向目标数组的指针，用来复制产生的C字符串
*             iMaxSize:被复制到pcStr的最大字符数
*             pcFormat:这是C字符串，包含了普通字符和特殊格式说明符的任何组合。这些
*             格式说明符由函数替换为表示tm中所指定时间的相对应值。格式说明符如下：
                    说明符	        替换为	                            实例
                    %a	        缩写的星期几名称	                    Sun
                    %A	        完整的星期几名称	                    Sunday
                    %b	        缩写的月份名称	                        Mar
                    %B	        完整的月份名称	                        March
                    %c	        日期和时间表示法	                    Sun Aug 19 02:56:02 2012
                    %d	        一月中的第几天（01-31）	                19
                    %H	        24 小时格式的小时（00-23）	            14
                    %I	        12 小时格式的小时（01-12）	            05
                    %j	        一年中的第几天（001-366）	            231
                    %m	        十进制数表示的月份（01-12）	            08
                    %M	        分（00-59）	                    55
                    %p	        AM 或 PM 名称	                    PM
                    %S	        秒（00-61）	                    02
                    %U	        一年中的第几周，以第一个星期日作为第一周的第一天（00-53）                                     33
                    %w	        十进制数表示的星期几，星期日表示为 0（0-6）	                                        4
                    %W	        一年中的第几周，以第一个星期一作为第一周的第一天（00-53）	                                    34
                    %x	        日期表示法	                        08/19/12
                    %X	        时间表示法	                        02:50:06
                    %y	        年份，最后两个数字（00-99）	            01
                    %Y	        年份	                            2012
                    %Z	        时区的名称或缩写	                    CDT
                    %%	        一个 % 符号	                        %
              pstTime:指向tm结构的指针，该结构包含了一个被分解为以下各部分的日历时间
*   输出参数：无
*   返 回 值：如果产生的 C 字符串小于 size 个字符（包括空结束字符），
*             则会返回复制到 str 中的字符总数（不包括空结束字符），否则返回零。
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
size_t StrFTime(CHAR *pcStr, size_t iMaxSize, const CHAR *pcFormat, const struct tm *pstTime)
{
    return strftime(pcStr, iMaxSize, pcFormat, pstTime);
}

/*******************************************************************************
*   函 数   名：GetTimeOfDay
*   功     能：获取系统当前的时间及地区信息，其中时间可以达到微秒级
*   输入参数：pstTimeVal:获取时间的结构体
*             pstTimeZone:获取时区信息的结构体
*   输出参数：无
*   返 回 值：返回当前系统的时间及地区信息
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 GetTimeOfDay(struct timeval *pstTimeVal, struct timezone *pstTimeZone)
{
    return gettimeofday(pstTimeVal, pstTimeZone);
}

/*******************************************************************************
*   函 数   名：SetTimeOfDay
*   功     能：设置系统当前的时间和时区
*             把目前时间设成由pstTimeVal所指的结构信息，当地时区信息则设成pstTimeZone所指的结构
*   输入参数：pstTimeVal:时间的结构体
*             pstTimeZone:时区信息的结构体
*   输出参数：无
*   返 回 值：成功返回0，失败返回-1
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 SetTimeOfDay(struct timeval *pstTimeVal, struct timezone *pstTimeZone)
{
    return settimeofday(pstTimeVal, pstTimeZone);
}

/*******************************************************************************
*   函 数   名：CreateSelectTimer
*   功     能：创建一个以select为基础的定时器，超时后触发
*   输入参数：iSeconds:秒
*             iMesconds:毫秒
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
VOID CreateSelectTimer(INT32 iSeconds, INT32 iMesconds)
{
    struct timeval stTimeVal;  

    stTimeVal.tv_sec = iSeconds;  
    stTimeVal.tv_usec = iMesconds;  

    select(0, NULL, NULL, NULL, &stTimeVal);  
    printf("Hello Timer\n");  

    return ;  
}

/*******************************************************************************
*   函 数   名：CreateAlarmTimer
*   功     能：创建Alarm定时器，需要将定时器时间传入到超时处理函数中去
*			  实际测试并不具有定时作用
*   输入参数：iSeconds:秒
*             fSigTimeoutHandler:定时器超时处理函数，用户自定义
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
VOID CreateAlarmTimer(INT32 iSeconds, fSignalActionHandler fSigTimeoutHandler)
{
    /* 注册信号处理函数 */
    SignalActionWithParamRegister(SIGALRM, fSigTimeoutHandler);

    /* 触发alarm信号并将定时器时间作为参数传入 */
    SignalActionWithParamSend(getpid(), SIGALRM, iSeconds);

    return ;  
}

/*******************************************************************************
*   函 数   名：CreateAlarmStdTimer
*   功     能：创建Alarm定时器
*   输入参数：iSeconds:秒
*             fSigTimeoutHandler:定时器超时处理函数，用户自定义
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
VOID CreateAlarmStdTimer(INT32 iSeconds, fSignalHandler fSigTimeoutHandler)
{
    /* 注册信号处理函数 */
    SignalRegister(SIGALRM, fSigTimeoutHandler);

    /* 触发alarm信号 */
    SignalAlarmSend(iSeconds);

    return ;  
}

/*******************************************************************************
*   函 数   名：CreateTimer
*   功     能：创建定时器
*   输入参数：iStartDelay:启动间隔iStartDelay时间后开始装载定时器
*			  iTimerDelay:定时器间隔周期
*             fTimerHandler:定时器处理回调函数
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
VOID CreateTimer(INT32 iStartDelay, INT32 iTimerDelay, fTimerProcessHandler fTimerHandler)
{
	// int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);
	// clockid：CLOCK_REALTIME,CLOCK_MONOTONIC,CLOCK_PROCESS_CPUTIME_ID,CLOCK_THREAD_CPUTIME_ID
	// evp：存放环境值的地址,结构成员说明了定时器到期的通知方式和处理方式等
	// timerid：定时器标识符
	timer_t iTimerId;
	struct sigevent stSigEvp;
	struct itimerspec stItimer;
	
	memset(&stSigEvp, 0, sizeof(struct sigevent));	//清零初始化

	stSigEvp.sigev_value.sival_int = 111;			//也是标识定时器的，这和timerid有什么区别？回调函数可以获得
	stSigEvp.sigev_notify = SIGEV_THREAD;			//线程通知的方式，派驻新线程
	stSigEvp.sigev_notify_function = fTimerHandler;	//线程函数地址

	if (timer_create(CLOCK_REALTIME, &stSigEvp, &iTimerId) == -1)
	{
		printf("Fail to timer_create");
		return;
	}

	// int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value,struct itimerspec *old_value);
	// timerid：定时器标识
	// flags：0表示相对时间，1表示绝对时间，通常使用相对时间
	// new_value：定时器的新初始值和间隔，如下面的it
	// old_value：取值通常为0，即第四个参数常为NULL,若不为NULL，则返回定时器的前一个值

	// 第一次间隔stItimer.it_value这么长,以后每次都是stItimer.it_interval这么长,就是说stItimer.it_value变0的时候会装载stItimer.it_interval的值
	// stItimer.it_interval可以理解为周期
	stItimer.it_interval.tv_sec = iTimerDelay;		//间隔时间
	stItimer.it_interval.tv_nsec = 0;
	stItimer.it_value.tv_sec = iStartDelay; 		//启动间隔时间
	stItimer.it_value.tv_nsec = 0;

	if (timer_settime(iTimerId, 0, &stItimer, NULL) == -1)
	{
		printf("Fail to timer_settime");
		return;
	}

    return ;  
}

/*******************************************************************************
*   函 数   名：CreateSignalTimer
*   功     能：创建定时器
*   输入参数：iStartDelay:启动间隔iStartDelay时间后开始装载定时器
*			  iTimerDelay:定时器间隔周期
*             fTimerHandler:定时器处理回调函数
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 CreateSignalTimer(INT32 iStartDelay, INT32 iTimerDelay, timer_t *pstTimerId, fSignalHandler fTimerHandler)
{
	// int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
	// signum：指定的信号编号，可以指定SIGKILL和SIGSTOP以外的所有信号编号
	// act结构体：设置信号编号为signum的处理方式
	// oldact结构体：保存上次的处理方式
	//
	// struct sigaction 
	// {
	// 		void (*sa_handler)(int); 		//信号响应函数地址
	// 		void (*sa_sigaction)(int, siginfo_t *, void *);	 //但sa_flags为SA——SIGINFO时才使用
	// 		sigset_t sa_mask;		 //说明一个信号集在调用捕捉函数之前，会加入进程的屏蔽中，当捕捉函数返回时，还原
	// 		int sa_flags;
	// 		void (*sa_restorer)(void);	//未用
	// };
	//
	struct sigevent stSigEvp;
	struct sigaction stSigAction;
	struct itimerspec stItimer;
	INT32 iTimerRet = 0;
	
	memset(&stSigAction, 0, sizeof(stSigAction));
	stSigAction.sa_handler = fTimerHandler;
	stSigAction.sa_flags = 0;

	// int sigaddset(sigset_t *set, int signum);  //将signum指定的信号加入set信号集
	// int sigemptyset(sigset_t *set);			//初始化信号集

	sigemptyset(&stSigAction.sa_mask);

	if (sigaction(SIGUSR1, &stSigAction, NULL) == -1)
	{
		printf("Fail to sigaction.\n");
		return 0;
	}

	memset(&stSigEvp, 0, sizeof(struct sigevent));
	stSigEvp.sigev_signo = SIGUSR1;
	stSigEvp.sigev_notify = SIGEV_SIGNAL;
	iTimerRet = timer_create(CLOCK_REALTIME, &stSigEvp, pstTimerId);
	if (iTimerRet == -1)
	{
		printf("Fail to timer_create.\n");
		return 0;
	}

	stItimer.it_interval.tv_sec = iTimerDelay;
	stItimer.it_interval.tv_nsec = 0;
	stItimer.it_value.tv_sec = iStartDelay;
	stItimer.it_value.tv_nsec = 0;
	if (timer_settime(*pstTimerId, 0, &stItimer, 0) == -1)
	{
		printf("Fail to timer_settime.\n");
		return 0;
	}

    return iTimerRet;  
}

/*******************************************************************************
*   函 数   名：SetTimer
*   功     能：设置定时器
*   输入参数：iTimerId:定时器ID
*			  iTimerInterval：定时器周期
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
VOID SetTimer(timer_t *pstTimerId, INT32 iStartInterval, INT32 iTimerInterval)
{
	struct itimerspec stItimer;
	
	stItimer.it_interval.tv_sec = iTimerInterval;
	stItimer.it_interval.tv_nsec = 0;
	stItimer.it_value.tv_sec = 0;
	stItimer.it_value.tv_nsec = iStartInterval;	//必须要设置，否则不能成功修改
	if (timer_settime(*pstTimerId, 0, &stItimer, 0) == -1)
	{
		printf("Fail to timer_settime.\n");
		return;
	}

	printf("SetTimer\n");

    return ;  
}


/*******************************************************************************
*   函 数   名：CancleTimer
*   功     能：取消定时器
*   输入参数：iTimerId:定时器ID
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
VOID CancleTimer(timer_t *pstTimerId)
{
	struct itimerspec stItimer;
	
	stItimer.it_interval.tv_sec = 0;
	stItimer.it_interval.tv_nsec = 0;
	stItimer.it_value.tv_sec = 0;
	stItimer.it_value.tv_nsec = 0;
	if (timer_settime(*pstTimerId, 0, &stItimer, 0) == -1)
	{
		printf("Fail to timer_settime.\n");
		return;
	}

	printf("CancleTimer\n");

    return ;  
}

/*******************************************************************************
*   函 数   名：DeleteTimer
*   功     能：销毁定时器
*   输入参数：iTimerId:定时器ID
*   输出参数：无
*   返 回 值：成功返回0
*			  失败返回-1，并设置errno
*   作     者：zhanxc
*   创建日期：018-7-23
*   修改历史：无
*******************************************************************************/
INT32 DeleteTimer(timer_t *pstTimerId)
{
	INT32 iRet = 0;
	
	iRet = timer_delete(*pstTimerId);

	printf("DeleteTimer\n");

    return iRet;  
}

