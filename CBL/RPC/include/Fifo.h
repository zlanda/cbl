/*******************************************************************************
*   文  件 名：Fifo.h
*   功     能：管道对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-14
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_FIFO_H_
#define  _CBL_RPC_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/* 内部头文件 */
#include "Types.h"

/* 定义错误码 */
#define CBL_MKFIFO_FILE_ERR                     -2

#define CBL_OPENFIFO_FILE_ERR                   -2

#define CBL_CLOSEFIFO_ERR                       -2

#define CBL_READFIFO_PARAM_ERR                  -2

#define CBL_WRITEFIFO_PARAM_ERR                 -2

/* 定义FIFO对外接口 */
/* 创建有名管道 */
INT32 CreateMkFifo(const CHAR *pcFileName, mode_t iMode);

/* 打开有名管道 */
INT32 OpenFifo(const CHAR *pcFifoName, INT32 iFlags);

/* 关闭有名管道 */
VOID CloseFifo(INT32 iFifoFd);

/* 读管道 */
INT32 ReadFifo(INT32 iFifoFd, CHAR *pcBuffer, UINT32 uiBufferSize);

/* 写管道 */
INT32 WriteFifo(INT32 iFifoFd, CHAR *pcBuffer, UINT32 uiBufferSize);

#ifdef __cplusplus
}
#endif

#endif

