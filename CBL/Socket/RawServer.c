#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdlib.h>  
#include <syslog.h>  
#include <errno.h> 

//定义监听最大客户端数目
#define MAX_LISTEN_NUM						5
//定义socket发送缓存大小
#define SEND_BUF_SIZE						1024
//定义接收缓存大小
#define RECV_BUF_SIZE						1024
//定义监听端口号
#define LISTEN_PORT							1010

static char *pcBufStr = "1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n" \
						"1234567890qwertyuiopasdfghjklzxcvbnm\r\n";

int main()  
{  
	int iListenSock = 0;
	int iClientSock = 0;
	struct sockaddr_in stHostAddr;  
	struct sockaddr_in stClientAddr;  
	int iSockAddrLen = sizeof(stClientAddr);  
	char acSendBuf[SEND_BUF_SIZE] = {0};  
	char acRecvBuf[RECV_BUF_SIZE] = {0};  
	int iSendLen = 0;
	int iRetLen = 0;

	//清空地址内存
	memset((void *)&stHostAddr, 0, sizeof(stHostAddr));
	memset((void *)&stClientAddr, 0, sizeof(stClientAddr));

	//服务器端地址赋值，监听127.0.0.1:1010
	stHostAddr.sin_family = AF_INET;
	stHostAddr.sin_port = htons(LISTEN_PORT);
	stHostAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//创建socket
	iListenSock = socket(AF_INET, SOCK_STREAM, 0);  
	if (iListenSock < 0)  
	{
		//socket创建失败，显示失败原因
		perror("socket");
		//创建失败则该进程退出
		exit(1);  
	}

	//将socket与服务器端地址相绑定
	if (bind(iListenSock, (struct sockaddr *)&stHostAddr, sizeof(stHostAddr)) < 0)  
	{
		//绑定失败，显示失败原因
		perror("bind");
		//绑定失败则该进程退出
		exit(1);
	}

	//监听客户端，最大5个连接客户端
	if(listen(iListenSock, MAX_LISTEN_NUM) < 0)  
	{
		//监听失败，显示失败原因
		perror("listen");
		//监听失败则该进程退出
		exit(1);
	}

	//死循环，等待客户端连接	
	while (1)
	{
		//等待客户端连接
		iClientSock = accept(iListenSock, (struct sockaddr *)&stClientAddr, &iSockAddrLen);  
		if (iClientSock < 0)  
		{
			//客户端连接失败，显示失败原因
			perror("accept");
			//失败则该进程退出
			exit(1);
		}

		//组建发送信息
		sprintf(acSendBuf, "Welcome %s:%d here. %s\r\n", 
				inet_ntoa(stClientAddr.sin_addr.s_addr), 
				stClientAddr.sin_port,
				pcBufStr);

		printf("Send message to client: %s.\r\n", (char *)acSendBuf);

		//发送缓冲区信息长度
		iSendLen = strlen(acSendBuf) + 1;
		//发送后返回的实际发送长度
		iRetLen = 0;

		//向客户端发送信息
		iRetLen = send(iClientSock, (char *)acSendBuf, iSendLen, 0);  
		if (iRetLen < 0)  
		{
			//检查发送是否失败
			if (errno == EINTR)
			{
				iRetLen = 0;
			}
			else
			{
				exit(1);
			}  
		}

		//发送完成后，等待接收客户端信息
		//实际接收长度
		iRetLen = 0;
		
		//接收缓冲区
		iRetLen = recv(iClientSock, (char *)acRecvBuf, RECV_BUF_SIZE, 0) ;  
		if (iRetLen < 0)
		{
			//接收失败
			if (errno == EINTR)
			{
				iRetLen = 0;
			}
			else
			{
				exit(1);
			}
		}
		
		//显示接收到的信息
		printf("Receive message from client: %s.\r\n", (char *)acRecvBuf);
		//关闭客户端socket
		close(iClientSock);
	}

	//关闭服务器端socket
	close(iListenSock);

	return 0;
}

