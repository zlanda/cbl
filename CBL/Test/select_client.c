#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Types.h"

int main(int argc, char **argv)
{
    INT32 iClientFd;
    struct sockaddr_in stServAddr;
    CHAR acBuffer[1024];
    INT32 iLen = 0;

    if (argc != 2)
    {
        printf("usage: ./SelectClient <ipaddress>\n");
        return 0;
    }

    // 创建socket
    if ((iClientFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket error: %s(errno: %d)", strerror(errno), errno);
        return 0;
    }
    
    printf("sockfd: %d\n", iClientFd);

    // 指定本机传输端口和目的地址
    bzero(&stServAddr, sizeof(stServAddr));
    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(6666);
    
    if (inet_pton(AF_INET, argv[1], &stServAddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", argv[1]);
        return 0;
    }

    // 尝试连接服务器
    if (connect(iClientFd, (struct sockaddr *)&stServAddr, sizeof(stServAddr)) < 0)
    {
        printf("connect socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    bzero(acBuffer, 1024);
    recv(iClientFd, acBuffer, 1024, 0);
    printf("recv: %s\n", acBuffer);
    
    bzero(acBuffer, sizeof(acBuffer));
    strcpy(acBuffer, "this is client.\n");
    send(iClientFd, acBuffer, sizeof(acBuffer), 0);
    
    while (1)
    {
        bzero(acBuffer, sizeof(acBuffer));
        fgets(acBuffer, sizeof(acBuffer), stdin);
        
        iLen = strlen(acBuffer);
        acBuffer[iLen] = '\0';
        send(iClientFd, acBuffer, sizeof(acBuffer), 0);
        printf("I have send buffer.\n");
    }

    close(iClientFd);
    
    return 0;
}

