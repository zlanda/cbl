#include "Socket.h"

/* 定义socket path路径 */
#define CBL_ADDR_PATH				"/tmp/socketrpc"

/* 定义接收缓存大小 */
#define CBL_SEND_BUFF_MAX			100

int main(int argc, char **argv)
{
	INT32 iClientSockFd = 0;
    CHAR buffer[BUFSIZ];
    CHAR acRecvBuff[CBL_SEND_BUFF_MAX];
	
    /* 创建socket客户端 */
    iClientSockFd = CreateUnixClientSocket(CBL_ADDR_PATH);
    if (iClientSockFd < 0)
	{
		return -1;
	}

	while (1)
    {
        printf("Send: \n");
        fgets(buffer, BUFSIZ, stdin);
        strcpy(acRecvBuff, buffer);
 
        // 向服务器端发送消息
        if (SendMessage(iClientSockFd, (CHAR *)acRecvBuff, CBL_SEND_BUFF_MAX, 0) < 0)
        {
            fprintf(stderr, "SendMessage failed\n");
            exit(EXIT_FAILURE);
        }
 
        // 输入end结束输入
        if (strncmp(acRecvBuff, "end", 3) == 0)
        {
            break;
        }
 
        sleep(1);
    }

    /* 关闭客户端socket */
    CloseUnixSocket(iClientSockFd);

    return 0;
}
