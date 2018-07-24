/*******************************************************************************
*   文  件 名：Thread.h
*   功     能：线程对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-14
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_COMMON_THREAD_H_
#define  _CBL_COMMON_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

/* 内部头文件 */
#include "Types.h"

/* 定义线程执行函数 */
typedef VOID* (*fPthreadStart)(VOID *);

/* 定义清理线程回调后执行函数 */
typedef VOID* (*fCleanUpHandler)(VOID *);

/* 线程对外接口 */
pthread_t PthreadSelf(VOID);

INT32 PthreadEqual(pthread_t iTid1, pthread_t iTid2);

INT32 PthreadCreate(pthread_t *pTid, const pthread_attr_t *pstAttr, 
                    fPthreadStart fPthreadStartHandler, VOID *pvArg);

VOID PthreadExit(VOID *pvRVal);

VOID PthreadJoin(pthread_t iTid, VOID **pvRVal);

INT32 PthreadCancel(pthread_t iTid);

/* 注释掉的原因在于编译不能通过，很奇怪 */
#if 0
VOID PthreadCleanupPush(fCleanUpHandler fCleanUpProcessHandler, VOID *pvArg);

VOID PthreadCleanupPop(INT32 iExecute);
#endif

INT32 PthreadMutexInit(pthread_mutex_t *pstMutex, const pthread_mutexattr_t *pstAttr);

INT32 PthreadMutexDestroy(pthread_mutex_t *pstMutex);

INT32 PthreadMutexLock(pthread_mutex_t *pstMutex);

INT32 PthreadMutexTryLock(pthread_mutex_t *pstMutex);

INT32 PthreadMutexUnLock(pthread_mutex_t *pstMutex);

INT32 PthreadRwLockInit(pthread_rwlock_t *pstRwLock, const pthread_rwlockattr_t *pstRwLockAttr);

INT32 PthreadRwLockDestroy(pthread_rwlock_t *pstRwLock);

INT32 PthreadRwLockRdLock(pthread_rwlock_t *pstRwLock);

INT32 PthreadRwLockTryRdLock(pthread_rwlock_t *pstRwLock);

INT32 PthreadRwLockWrLock(pthread_rwlock_t *pstRwLock);

INT32 PthreadRwLockTryWrLock(pthread_rwlock_t *pstRwLock);

INT32 PthreadRwLockUnLock(pthread_rwlock_t *pstRwLock);

INT32 PthreadAttrInit(pthread_attr_t *pstAttr);

INT32 PthreadAttrDestroy(pthread_attr_t *pstAttr);

INT32 PthreadDetach(pthread_t iTid);

INT32 PthreadAttrSetdetachstate(pthread_attr_t *pstAttr, INT32 iDetachState);

INT32 PthreadAttrGetdetachstate(pthread_attr_t *pstAttr, INT32 *piDetachState);

INT32 PthreadAttrSetstacksize(pthread_attr_t *pstAttr, size_t iStackSize);

INT32 PthreadAttrGetstacksize(pthread_attr_t *pstAttr, size_t *piStackSize);

#ifdef __cplusplus
}
#endif

#endif

