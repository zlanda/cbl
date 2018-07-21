/*******************************************************************************
*   文  件 名：Socket.c
*   功     能：封装Unix Socket IPC功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/

#include "Socket.h"

/*******************************************************************************
*   函 数   名：Socket_NonBlock_Set
*   功     能：设置socket fd为非阻塞模式
*   输入参数：iSockFd:socket文件描述符
*   输出参数：无
*   返 回 值：CBL_FALSE:设置失败
*             CBL_TRUE:设置成功
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
static BOOL Socket_NonBlock_Set(INT32 iSockFd)  
{  
    INT32 iSockOptions;  

    iSockOptions = fcntl(iSockFd, F_GETFL);  
    if (iSockOptions < 0)  
    {  
		return CBL_FALSE;
    }
	
    iSockOptions = (iSockOptions | O_NONBLOCK);  
    if (fcntl(iSockFd, F_SETFL, iSockOptions) < 0)  
    {  
		return CBL_FALSE; 
    }

	return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：CreateUnixServerSocket
*   功     能：创建Unix Socket服务器端
*   输入参数：pcSockPath:PATH文件路径
*             uiMaxConnNum:最大连接个数
*   输出参数：无
*   返 回 值：>0:socket文件描述符
*             <0:创建失败
*   作     者：zhanxc
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
INT32 CreateUnixServerSocket(CHAR *pcSockPath, UINT32 uiMaxConnNum)
{
	INT32 iSockFd = -1;
	BOOL bSockOptions = CBL_FALSE;
	struct sockaddr_un stSockAddr;

	if (CBL_NULL == pcSockPath)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_PATH_VALID;
	}

	/* 创建socket */
	iSockFd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (iSockFd < 0)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_CREATE_FAILED;
	}

#if 0
	/* 设置socket文件描述符为非阻塞模式 */
	bSockOptions = Socket_NonBlock_Set(iSockFd);
	if (!bSockOptions)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_MODE_FAILED;
	}
#endif

	/* 设置socket地址 */
	bzero(&stSockAddr, sizeof(stSockAddr));
	stSockAddr.sun_family = AF_UNIX;
	memcpy(stSockAddr.sun_path, pcSockPath, strlen(pcSockPath) + 1);

	/* socket与地址绑定 */
	if (bind(iSockFd, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)) < 0)
	{  
		return CBL_CREATE_UNIX_SERVER_SOCKET_BIND_FAILED;
    }

	/* 监听客户端连接 */
	if (listen(iSockFd, uiMaxConnNum) < 0)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_LISTEN_FAILED;
	}

	return iSockFd;
}

/*******************************************************************************
*   函 数   名：CreateUnixClientSocket
*   功     能：创建Unix Socket客户端
*   输入参数：pcSockPath:PATH文件路径
*   输出参数：无
*   返 回 值：>0:socket文件描述符
*             <0:创建失败
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
INT32 CreateUnixClientSocket(CHAR *pcSockPath)
{
	INT32 iSockFd = -1;
	INT32 iOnFlag = 1;
	struct sockaddr_un stServerAddr;

	if (CBL_NULL == pcSockPath)
	{
        return CBL_CREATE_UNIX_CLIENT_SOCKET_PATH_VALID;  
	}

	/* 创建socket */
	if((iSockFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {  
        return CBL_CREATE_UNIX_CLIENT_SOCKET_CREATE_FAILED;  
    }

	setsockopt(iSockFd, SOL_SOCKET, SO_REUSEADDR, &iOnFlag, sizeof(iOnFlag));

	/* 设置地址 */
	memset(&stServerAddr, 0, sizeof(stServerAddr));
	stServerAddr.sun_family = AF_UNIX;  
	memcpy(stServerAddr.sun_path, pcSockPath, strlen(pcSockPath) + 1);

	/* 连接服务器端 */
    if(connect(iSockFd, (struct sockaddr*)&stServerAddr, sizeof(stServerAddr)) < 0)
    {  
        return CBL_CREATE_UNIX_CLIENT_SOCKET_CONNECT_FAILED;  
    }

	return iSockFd;
}

/*******************************************************************************
*   函 数   名：AcceptUnixSocket
*   功     能：等待客户端连接
*   输入参数：iSockFd:socket文件描述符
*   输出参数：无
*   返 回 值：从已完成连接队列返回下一个已完成连接，如果已完成连接队列为空，
*			  那么进程被投入睡眠
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
INT32 AcceptUnixSocket(INT32 iSockFd)
{
	struct sockaddr_un stRemoteSockAddr;
	socklen_t iSockAddrLen = 0;
	INT32 iConnectFd = -1;

	/* 文件描述符非法，可能已经关闭 */
	if (iSockFd < 0)
	{
		return CBL_ACCEPT_FAILED;
	}
	
	memset(&stRemoteSockAddr, 0, sizeof(stRemoteSockAddr));
	iSockAddrLen = sizeof(stRemoteSockAddr);

	iConnectFd = accept(iSockFd, (struct sockaddr *)&stRemoteSockAddr, &iSockAddrLen);
	if (iConnectFd < 0)
	{
		return CBL_ACCEPT_FAILED;
	}
	
	return iConnectFd;
}


/*******************************************************************************
*   函 数   名：CloseUnixSocket
*   功     能：关闭Unix Socket
*   输入参数：iSockFd:socket文件描述符
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
VOID CloseUnixSocket(INT32 iSockFd)
{
	/* 文件描述符非法，可能已经关闭 */
	if (iSockFd < 0)
	{
		return;
	}

	/* 关闭socket文件描述符 */
	close(iSockFd);
	
	return;
}

/*******************************************************************************
*   函 数   名：SendMessage
*   功     能：发送消息
*   输入参数：iSockFd:socket文件描述符
*			  pcBuffer:待发送的消息内容
*			  iBufferLen:待发送的消息长度
*			  iFlags:调用执行方式
*   输出参数：无
*   返 回 值：发送成功返回发送数据的总数，注意这个数字可能小于iBufferLen中所规定的大小
*			  失败返回错误码
*	错 误 码: WSANOTINITIALISED：在使用此API之前应首先成功地调用WSAStartup()。
*			  WSAENETDOWN：WINDOWS套接口实现检测到网络子系统失效。
*			  WSAEACESS：要求地址为广播地址，但相关标志未能正确设置。
*			  WSAEINTR：通过一个WSACancelBlockingCall()来取消一个（阻塞的）调用。
*			  WSAEINPROGRESS：一个阻塞的WINDOWS套接口调用正在运行中。
*			  WSAEFAULT：buf参数不在用户地址空间中的有效位置。
*			  WSAENETRESET：由于WINDOWS套接口实现放弃了连接，故该连接必需被复位。
*			  WSAENOBUFS：WINDOWS套接口实现报告一个缓冲区死锁。
*			  WSAENOTCONN：套接口未被连接。
*			  WSAENOTSOCK：描述字不是一个套接口。
*			  WSAEOPNOTSUPP：已设置了MSG_OOB，但套接口非SOCK_STREAM类型。
*			  WSAESHUTDOWN：套接口已被关闭。一个套接口以1或2的how参数调用shutdown()关闭后，无法再用send()函数。
*			  WSAEWOULDBLOCK：
*			  WSAEMSGSIZE：套接口为SOCK_DGRAM类型，且数据报大于WINDOWS套接口实现所支持的最大值。
*			  WSAEINVAL：套接口未用bind()捆绑。
*			  WSAECONNABORTED：由于超时或其他原因引起虚电路的中断。
*			  WSAECONNRESET：虚电路被远端复位。
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
INT32 SendMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags)
{
	INT32 iSendRet = 0;

	/* 校验参数 */
	if (CBL_NULL == pcBuffer || iBufferLen <= 0)
	{
		return CBL_SEND_MESSAGE_MSG_PARAM_VALID;
	}
	
	/* 文件描述符非法，可能已经关闭 */
	if (iSockFd < 0)
	{
		return CBL_SEND_MESSAGE_MSG_SOCKET_VALID;
	}

	/* 发送消息 */
	iSendRet = send(iSockFd, pcBuffer, iBufferLen, iFlags);
	
	return iSendRet;
}

/*******************************************************************************
*   函 数   名：RecvMessage
*   功     能：接收消息
*   输入参数：iSockFd:socket文件描述符
*			  pcBuffer:接收消息存放缓存
*			  iBufferLen:接收消息缓存长度
*			  iFlags:调用执行方式
*   输出参数：无
*   返 回 值：如果没有错误发生，返回读入的字节数
*			  如果连接已中止，返回0
*			  否则返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()获取相应错误代码。
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
INT32 RecvMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags)
{
	INT32 iRecvLen = 0;

	/* 校验参数 */
	if (CBL_NULL == pcBuffer || iBufferLen <= 0)
	{
		return CBL_SEND_MESSAGE_MSG_PARAM_VALID;
	}
	
	/* 文件描述符非法，可能已经关闭 */
	if (iSockFd < 0)
	{
		return CBL_SEND_MESSAGE_MSG_SOCKET_VALID;
	}

	/* 发送消息 */
	iRecvLen = recv(iSockFd, pcBuffer, iBufferLen, iFlags);
	
	return iRecvLen;
}

