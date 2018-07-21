/*******************************************************************************
*   文  件 名：Sem.h
*   功     能：信号量对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_SEM_H_
#define  _CBL_RPC_SEM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* 内部头文件 */
#include "Types.h"

/* 定义一个函数名替换 */
#define ControlSemaphore                semctl

/* 定义信号量对外接口 */
/* 创建信号量 */
INT32 CreateSemaphore(key_t iKey, INT32 iSemNums, INT32 iSemFlag);

/* 操作信号量 */
INT32 OperationSemaphore(INT32 iSemId, struct sembuf astSemOpArray[], size_t iNOps);

/* 初始化信号量的值 */
INT32 InitSemaphoreValue(INT32 iSemId, INT32 iValue);

/* 设置信号量的值 */
INT32 SetSemaphoreValue(INT32 iSemId, INT32 iSemVal);

/* 获取信号量的值 */
INT32 GetSemaphoreValue(INT32 iSemId);

/* 删除信号量 */
INT32 DelSemaphore(INT32 iSemId);

#ifdef __cplusplus
}
#endif

#endif

