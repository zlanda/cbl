#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <syslog.h>  
#include <errno.h>  
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>

//服务器端地址
#define SERVER_IP						"127.0.0.1"
//端口
#define UDP_PORT						2345
//发送缓冲区长度
#define SEND_BUFFER_LEN					256
//接收缓冲区长度
#define RECV_BUFFER_LEN					256

int main()
{
	int iSockFd;
	struct sockaddr_in stServerAddr;
	int iAddrLen = sizeof(struct sockaddr_in);
	char acRecvBuffer[RECV_BUFFER_LEN];
	struct sockaddr_in stRemoteAddr;
    int iRemoteAddrLen = sizeof(stRemoteAddr); 
	
	/* 建立socket */
	if ((iSockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("socket");
		exit(1);
	}
	
	/* 填写服务器端地址 */
	bzero(&stServerAddr, sizeof(stServerAddr));
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(UDP_PORT);
	stServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	/* 建立socket与服务器端地址绑定关系 */
    if (bind(iSockFd, (struct sockaddr *)&stServerAddr, iAddrLen) < 0)
    {
        perror("bind");
		exit(1);
    }
	
	while (1)
	{
		/* 接收客户端发送的消息 */
		recvfrom(iSockFd, 
				 (char *)acRecvBuffer, 
				 sizeof(acRecvBuffer), 
				 0, 
				 &stRemoteAddr, 
				 &iRemoteAddrLen);
				 
		printf("Receive: %s.\r\n", acRecvBuffer);
		
		/* 向客户端发送消息 */
		sendto(iSockFd, 
			   acRecvBuffer, 
			   sizeof(acRecvBuffer), 
			   0, 
			   &stRemoteAddr, 
			   iRemoteAddrLen);
	}

	return 0;
}

