#include "Socket.h"

/* 定义socket path路径 */
#define CBL_ADDR_PATH				"/tmp/socketrpc"

/* 定义最大连接数 */
#define CBL_CONN_MAX				10

/* 定义接收缓存大小 */
#define CBL_RECV_BUFF_MAX			100

int main(int argc, char **argv)
{
	INT32 iServerSockFd = 0;
	CHAR acRecvBuff[CBL_RECV_BUFF_MAX];
	INT32 iConnSockFd = 0;

	/* 创建socket服务器端 */
	iServerSockFd = CreateUnixServerSocket(CBL_ADDR_PATH, CBL_CONN_MAX);
	if (iServerSockFd < 0)
	{
		return -1;
	}

	iConnSockFd = AcceptUnixSocket(iServerSockFd);
	/* 监听客户端连接 */
	if (iConnSockFd < 0)
	{
		fprintf(stderr, "Accept failed width erro: %d", errno);
		return -1;
	}

	while (1)
    {
    	//接收消息
        if (RecvMessage(iConnSockFd, (CHAR *)acRecvBuff, CBL_RECV_BUFF_MAX, 0) < 0)
        {
            fprintf(stderr, "RecvMessage failed width erro: %d", errno);
        }
 
        printf("Receive: %s\n", acRecvBuff);
 
        // 遇到end结束
        if (strncmp(acRecvBuff, "end", 3) == 0)
        {
            break;
        }

        sleep(1);
    }

	/* 关闭socket */
    CloseUnixSocket(iConnSockFd);
    
	/* 关闭socket */
    CloseUnixSocket(iServerSockFd);

    return 0;
}
