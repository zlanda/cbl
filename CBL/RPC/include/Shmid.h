/*******************************************************************************
*   文  件 名：Shmid.h
*   功     能：共享内存对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_SHMID_H_
#define  _CBL_RPC_SHMID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

/* 内部头文件 */
#include "Types.h"

/* 定义共享内存对外接口 */
INT32 CreateShmid(key_t iKey, size_t iSize, INT32 iFlag);

VOID *AtachShmid(INT32 iShmid, const VOID *pvShmAddr, INT32 iFlag);

INT32 DetachShmid(const VOID *pvShmAddr);

INT32 ControlShmid(INT32 iShmid, INT32 iCmd, struct shmid_ds *pstBuff);

#ifdef __cplusplus
}
#endif

#endif

