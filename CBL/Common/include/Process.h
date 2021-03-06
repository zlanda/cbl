/*******************************************************************************
*   文  件 名：Process.h
*   功     能：进程对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-14
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_COMMON_PROCESS_H_
#define  _CBL_COMMON_PROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 内部头文件 */
#include "Types.h"

/* 系统头文件 */
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* 进程对外接口 */
/* fork一个新进程 */
pid_t CBLFork(VOID);

/* 获取进程ID */
pid_t CBLGetPid(VOID);

/* 获取进程的父进程ID */
pid_t CBLGetPPid(VOID);

#ifdef __cplusplus
}
#endif

#endif

