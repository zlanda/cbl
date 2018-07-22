/*******************************************************************************
*   文  件 名：Epoll.h
*   功     能：Epoll对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_RPC_EPOLL_H_
#define  _CBL_RPC_EPOLL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 系统头文件 */
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/resource.h>

/* 内部头文件 */
#include "Types.h"

/* 定义错误码 */
#define CBL_EPOLL_ERR					-1

/* 定义文件描述符获取模式错误 */
#define CBL_FCTL_ERR					-1

/* 文件描述符设置非阻塞模式成功 */
#define CBL_FCTL_SUCC					0

/* 定义Epoll对外接口 */
/* 创建Epoll句柄 */
INT32 CreateEpoll(UINT32 uiMaxEventsNum);

/* 关闭Epoll */
VOID CloseEpoll(INT32 iEpollFd);

/* epoll的事件注册函数 */
INT32 ControlEpoll(INT32 iEpollFd, INT32 iOp, INT32 iFd, struct epoll_event *pstEvent);

/* 等待事件触发 */
INT32 WaitEpoll(INT32 iEpollFd, struct epoll_event *pstEvents, INT32 iMaxEvents, INT32 iTimeout);

/* 设置文件描述符为非阻塞模式 */
INT32 SetFdNonBlocking(INT32 iSockFd);

#ifdef __cplusplus
}
#endif

#endif

