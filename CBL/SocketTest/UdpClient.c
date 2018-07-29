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
	char acSendbuffer[SEND_BUFFER_LEN] = "123456789asdfgghjkkasdfghzxcvnnn";
	char acRecvBuffer[RECV_BUFFER_LEN];
	
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

	/* 向服务器发送消息 */
	sendto(iSockFd, 
			(char *)acSendbuffer, sizeof(acSendbuffer), 
			0, 
			(struct addr *)&stServerAddr, 
			iAddrLen);

	/* 从服务器端接收消息 */
	recvfrom(iSockFd, 
			 acRecvBuffer, 
			 sizeof(acRecvBuffer), 
			 0 , 
			 (struct addr *)&stServerAddr, 
			 &iAddrLen);

	/* 显示接收到的消息 */
	printf("receive from %s data: %s\n", inet_ntoa(stServerAddr.sin_addr), (char *)acRecvBuffer);

	return 0;
}

