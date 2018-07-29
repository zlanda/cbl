#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <syslog.h>  
#include <errno.h>  
#include <stdlib.h>

//定义监听最大客户端数目
#define MAX_LISTEN_NUM					5
//定义socket发送缓存大小
#define SEND_BUF_SIZE					1024
//定义接收缓存大小
#define RECV_BUF_SIZE					1024
//定义监听端口号
#define SERVER_PORT						1010

static char *pcBufStr = "zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n" \
						"zxcvbnmasdfghjklqwertyuiop1234567890\r\n";

int main()  
{  
	int iClientSockFd = 0;  
	char acRecvBuf[RECV_BUF_SIZE] = {0};  
	char acSendBuf[SEND_BUF_SIZE] = {0};  
	int iRetLen = 0;  
	int iSendLen = 0;
	struct sockaddr_in stServerAddr;  

	//清空服务器端地址内存
	memset(&stServerAddr, 0, sizeof(stServerAddr));

	//服务器端地址设置
	stServerAddr.sin_family = AF_INET;
	inet_aton("127.0.0.1", (struct in_addr *)&stServerAddr.sin_addr);
	stServerAddr.sin_port = htons(SERVER_PORT);

	//创建客户端socket
	iClientSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (iClientSockFd < 0)  
	{
		//创建socket失败，显示失败原因，进程退出
		perror("socket");
		exit(1);
	}

	//连接服务器端	
	if (connect(iClientSockFd, (struct sockaddr *)&stServerAddr, sizeof(stServerAddr)) < 0)  
	{
		//连接失败，显示失败原因，进程退出
		perror("connect");
		exit(1);
	}
	
	//接收数据
	//接收数据实际接收长度
	iRetLen = 0;

	//接收信息
	iRetLen = recv(iClientSockFd, (char *)acRecvBuf, RECV_BUF_SIZE, 0);
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
		
	//显示客户端接收到的信息
	printf("Receive message form server: %s.\r\n", acRecvBuf);

	//组建发送缓冲区信息
	sprintf(acSendBuf, "Hello Server. %s\r\n", pcBufStr);
	
	//发送数据长度
	iSendLen = strlen(acSendBuf) + 1;
	//定义实际发送数据长度
	iRetLen = 0;

	printf("Send message to server: %s.\r\n", (char *)acSendBuf);	
	//发送信息
	iRetLen = send(iClientSockFd, (char *)acSendBuf, iSendLen, 0);
	if (iRetLen < 0)  
	{
		//发送失败
		if (errno == EINTR)
		{
			iRetLen = 0;
		}
		else
		{
			exit(1);
		}
	}

	//关闭客户端socket
	close(iClientSockFd);

	return 0;
}  
