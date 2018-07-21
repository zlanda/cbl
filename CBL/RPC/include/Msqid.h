/*******************************************************************************
*   文  件 名：Msqid.h
*   功     能：消息队列对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-14
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_MSQID_H_
#define  _CBL_RPC_MSQID_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

/* 内部头文件 */
#include "Types.h"

/* 定义消息队列对外接口 */
/* 创建消息队列 */
INT32 CreateMsgQid(key_t iKey, INT32 iMsgFlag);

/* 向消息队列中发送消息 */
INT32 SengMsgQid(INT32 iMsgId, const VOID *pvMsg, size_t iMsgSize, INT32 iMsgFlag);

/* 从消息队列中接收消息 */
INT32 RecvMsgQid(INT32 iMsgId, VOID *pvMsg, size_t iMsgSize, LONG lMsgType, INT32 iMsgFlag);

/* 控制消息队列 */
INT32 ControlMsgQid(INT32 iMsgId, INT32 iCmd, struct msqid_ds *pstBuffer);

#ifdef __cplusplus
}
#endif

#endif

