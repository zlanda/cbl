/*******************************************************************************
*   文  件 名：Pipe.h
*   功     能：匿名管道对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_PIPE_H_
#define  _CBL_RPC_PIPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <unistd.h>

/* 内部头文件 */
#include "Types.h"

/* 定义读匿名管道 */
#define CBL_PIPE_READ_FD						0

/* 定义写匿名管道 */
#define CBL_PIPE_WRITE_FD						1

/* 定义匿名管道 */
#define CBL_PIPE_NUM							2

/* 定义错误码 */
#define CBL_PIPE_READ_BUFFER_VALID				-1
#define CBL_PIPE_READ_BUFFER_LEN_VALID			-2

#define CBL_PIPE_WRITE_BUFFER_VALID				-1
#define CBL_PIPE_WRITE_BUFFER_LEN_VALID			-2

/* 定义pclose错误码 */
#define CBL_PCLOSE_FILE_VALID                   -2

/* 定义管道buffer长度 */
#define CBL_PIPE_BUFFER_LEN						200

/* 声明匿名管道对外接口 */
/* 创建匿名管道 */
INT32 CreatePipe(INT32 iPipe[CBL_PIPE_NUM]);

/* 从管道中读取数据 */
INT32 ReadPipe(INT32 iRdPipe, CHAR *cReadBuffer, UINT32 uiBufferLen);

/* 向管道中写入数据 */
INT32 WritePipe(INT32 iWrPipe, CHAR *cWriteMsg, UINT32 uiWriteMsgLen);

/* 关闭管道 */
VOID ClosePipe(INT32 iPipe);

/* 创建popen管道 */
FILE *CreatePopen(const CHAR *pcCommand, const CHAR *pcMode);

/* 从管道中读取数据 */
CHAR *ReadPopen(CHAR *pcBuff, INT32 iLen, FILE *pFile);

/* 关闭pclose管道 */
INT32 ClosePclose(FILE *pFile);

#ifdef __cplusplus
}
#endif

#endif

