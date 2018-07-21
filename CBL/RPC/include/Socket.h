/*******************************************************************************
*   文  件 名：Socket.h
*   功     能：Unix Socket IPC对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_FIFO_H_
#define  _CBL_RPC_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <errno.h>
#include <sys/un.h>

/* 内部头文件 */
#include "Types.h"

/* 定义错误码 */
/* 定义路径错误 */
#define CBL_CREATE_UNIX_SERVER_SOCKET_PATH_VALID		-1

/* 定义socket创建失败 */
#define CBL_CREATE_UNIX_SERVER_SOCKET_CREATE_FAILED		-2

/* 定义socket非阻塞模式失败 */
#define CBL_CREATE_UNIX_SERVER_SOCKET_MODE_FAILED		-3

/* 定义socket绑定失败 */
#define CBL_CREATE_UNIX_SERVER_SOCKET_BIND_FAILED		-4

/* 定义socket监听失败 */
#define CBL_CREATE_UNIX_SERVER_SOCKET_LISTEN_FAILED		-5

/* 定义路径错误 */
#define CBL_CREATE_UNIX_CLIENT_SOCKET_PATH_VALID		-1

/* 定义socket创建失败 */
#define CBL_CREATE_UNIX_CLIENT_SOCKET_CREATE_FAILED		-2

/* 定义socket连接失败 */
#define CBL_CREATE_UNIX_CLIENT_SOCKET_CONNECT_FAILED	-3

/* 等待客户端连接失败 */
#define CBL_ACCEPT_FAILED								-1

/* 定义发送消息时socket文件描述符非法 */
#define CBL_SEND_MESSAGE_MSG_SOCKET_VALID				-11111111

/* 定义发送消息时参数非法 */
#define CBL_SEND_MESSAGE_MSG_PARAM_VALID				-22222222

/* 定义socket对外接口 */
/* 创建服务器端socket */
INT32 CreateUnixServerTCPSocket(CHAR *pcSockPath, UINT32 uiMaxConnNum);

/* 创建客户端socket */
INT32 CreateUnixClientTCPSocket(CHAR *pcSockPath);

/* 等待客户端连接 */
INT32 AcceptUnixSocket(INT32 iSockFd);

/* 关闭socket */
VOID CloseUnixSocket(INT32 iSockFd);

/* 发送消息 */
INT32 SendMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags);

/* 接收消息 */
INT32 RecvMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags);

/* 创建服务器端socket */
INT32 CreateUnixServerUDPSocket(CHAR *pcSockPath);

/* 创建客户端socket */
INT32 CreateUnixClientUDPSocket(CHAR *pcSockPath);

#ifdef __cplusplus
}
#endif

#endif

