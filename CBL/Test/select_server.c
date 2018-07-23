#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Types.h"

int main()
{
    INT32 iServerFd;
    struct sockaddr_in stServAddr;
    fd_set stClientFdSet;       // 监控文件描述符集合
    INT32 iMaxSock;             // 监控文件描述符中最大的文件号
    struct timeval stTimeout;   // 超时返回时间
    INT32 aiClientSockFd[5];    // 存放活动的sockfd;
    INT32 iConnAmount = 0;      // 记录与服务器连接的客户端数
    CHAR acBuffer[1024];
    INT32 iRet;
    INT32 iIndex = 0;
    struct sockaddr_in stClientAddr;
    socklen_t iAddrSize = sizeof(stClientAddr);
    INT32 iClientSock = 0; 

    // 创建socket
    if ((iServerFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket error: %s(error: %d)\n", strerror(errno), errno);
        return 0;
    }

    // 配置服务器IP和端口
    bzero(&stServAddr, sizeof(stServAddr));
    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(6666);
    stServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // socket和地址绑定
    if (bind(iServerFd, (struct sockaddr *)&stServAddr, sizeof(stServAddr)) == -1)
    {
        printf("bind socket error: %s(error: %d)\n", strerror(errno), errno);
        return 0;
    }

    // 服务器开始监听
    if (listen(iServerFd, 10) == -1)
    {
        printf("listen socket error: %s(error: %d)\n", strerror(errno), errno);
        return 0;
    }
    
    bzero(&aiClientSockFd, sizeof(aiClientSockFd));
    iMaxSock = iServerFd;

    while (1)
    {
        FD_ZERO(&stClientFdSet);          // 清空集合
        FD_SET(iServerFd, &stClientFdSet); // 加入服务器描述符
        stTimeout.tv_sec = 30;
        stTimeout.tv_usec = 0;

        // 将活动的句柄加入到文件描述符集合中
        for (iIndex = 0; iIndex < 5; ++iIndex)
        {
            if (aiClientSockFd[iIndex] != 0)
            {
                FD_SET(aiClientSockFd[iIndex], &stClientFdSet);
            }
        }

        // 开始轮询
        iRet = select(iMaxSock + 1, &stClientFdSet, NULL, NULL, &stTimeout);
        if (iRet < 0)
        {
            perror("select error");
            break;
        }
        else if (iRet == 0)
        {
            printf("time out.\n");
            continue;
        }

        // 依次查询各个客户端是否有新消息发送过来
        for (iIndex = 0; iIndex < iConnAmount; ++iIndex)
        {
            if (FD_ISSET(aiClientSockFd[iIndex], &stClientFdSet))
            {
                iRet = recv(aiClientSockFd[iIndex], acBuffer, 1024, 0);
                if (iRet <= 0)
                {
                    printf("client[%d] close.\n\n", iIndex);
                    FD_CLR(aiClientSockFd[iIndex], &stClientFdSet);
                    close(aiClientSockFd[iIndex]);
                    aiClientSockFd[iIndex] = 0;
                }
                else
                {
                    printf("receive from client[%d]: %s\n\n", iIndex, acBuffer);
                }
            }
        }

        // 查询服务器是否有新的连接
        if (FD_ISSET(iServerFd, &stClientFdSet))
        {
            iClientSock = accept(iServerFd, (struct sockaddr_in *)&stClientAddr, &iAddrSize);

            if (iClientSock < 0)
            {
                perror("accept error");
                continue;
            }
            
            if (iConnAmount < 5)
            {
                aiClientSockFd[iConnAmount++] = iClientSock;
                bzero(acBuffer, sizeof(acBuffer));
                strcpy(acBuffer, "connected, welcome.");
                send(iClientSock, acBuffer, sizeof(acBuffer), 0);
                
                printf("new connection client[%d] %s: %d\n",
                       iConnAmount - 1, inet_ntoa(stClientAddr.sin_addr), ntohs(stClientAddr.sin_port));

                bzero(acBuffer, sizeof(acBuffer));
                iRet = recv(iClientSock, acBuffer, sizeof(acBuffer), 0);
                if (iRet < 0)
                {
                    printf("recv error\n");
                    close(iClientSock);
                    aiClientSockFd[iConnAmount - 1] = 0;
                    continue;
                }
                
                printf("recv: %s\n", acBuffer);
                
                if (iClientSock > iMaxSock)
                {
                    iMaxSock = iClientSock;
                }
            }
            else
            {
                printf("max connection, quit.\n");
                break;
            }
        }
    }
    for (iIndex = 0; iIndex < 5; ++iIndex)
    {
        if (aiClientSockFd[iIndex] != 0)
        {
            close(aiClientSockFd[iIndex]);
        }
    }
    
    close(iServerFd);
    
    return 0;
}

