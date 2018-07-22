#include "Epoll.h"

#define MAXEPOLLSIZE 10000
#define MAXLINE 10240

INT32 EpollServerHandle(INT32 iConnFd) 
{
    INT32 iReadNum;
    CHAR acBuffer[MAXLINE];
    iReadNum = read(iConnFd, acBuffer, MAXLINE);//读取客户端socket流

    if (iReadNum == 0) 
    {
        printf("client close the connection\n");
        close(iConnFd);
        
        return -1;
    } 
    
    if (iReadNum < 0)
    {
        perror("read error");
        close(iConnFd);
        
        return -1;
    }
    
    write(iConnFd, acBuffer, iReadNum);//响应客户端
    
    return 0;
}

int main(int argc, char **argv)
{
    INT32 iServPort = 6888;
    INT32 iListenQ = 1024;
    INT32 iListenFd;
    INT32 iConnFd;
    INT32 iEpollFd;
    INT32 iNFds;
    INT32 iIndex;
    INT32 iCurFds;
    INT32 iAcceptCount = 0;
    struct sockaddr_in stServAddr;
    struct sockaddr_in stClientAddr;
    socklen_t iSockLen = sizeof(struct sockaddr_in);
    struct epoll_event stEvent;
    struct epoll_event stEvents[MAXEPOLLSIZE];
    struct rlimit stRLimit;
    CHAR acBuffer[MAXLINE];
    INT32 iOption = 1;

    /* 设置每个进程允许打开的最大文件数 */
    stRLimit.rlim_max = stRLimit.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &stRLimit) == -1) 
    {
        perror("setrlimit error");
        return -1;
    }

    bzero(&stServAddr, sizeof(stServAddr));
    stServAddr.sin_family = AF_INET; 
    stServAddr.sin_addr.s_addr = htonl (INADDR_ANY);
    stServAddr.sin_port = htons (iServPort);

    iListenFd = socket(AF_INET, SOCK_STREAM, 0); 
    if (iListenFd == -1) 
    {
        perror("can't create socket file");
        return -1;
    }
    
    setsockopt(iListenFd, SOL_SOCKET, SO_REUSEADDR, &iOption, sizeof(iOption));

    if (SetFdNonBlocking(iListenFd) < 0)
    {
        perror("setnonblock error");
    }

    if (bind(iListenFd, (struct sockaddr *) &stServAddr, sizeof(struct sockaddr)) == -1) 
    {
        perror("bind error");
        return -1;
    } 
    if (listen(iListenFd, iListenQ) == -1) 
    {
        perror("listen error");
        return -1;
    }
    
    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    iEpollFd = CreateEpoll(MAXEPOLLSIZE);
    stEvent.events = EPOLLIN | EPOLLET;
    stEvent.data.fd = iListenFd;
    if (ControlEpoll(iEpollFd, EPOLL_CTL_ADD, iListenFd, &stEvent) < 0) 
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", iListenFd);
        return -1;
    }
    
    iCurFds = 1;

    printf("epollserver startup,port %d, max connection is %d, backlog is %d\n", iServPort, MAXEPOLLSIZE, iListenQ);

    for (;;) 
    {
        /* 等待有事件发生 */
        iNFds = WaitEpoll(iEpollFd, stEvents, iCurFds, -1);
        if (iNFds == -1)
        {
            perror("epoll_wait");
            continue;
        }
        
        /* 处理所有事件 */
        for (iIndex = 0; iIndex < iNFds; ++iIndex)
        {
            if (stEvents[iIndex].data.fd == iListenFd) 
            {
                iConnFd = accept(iListenFd, (struct sockaddr *)&stClientAddr,&iSockLen);
                if (iConnFd < 0) 
                {
                    perror("accept error");
                    continue;
                }

                sprintf(acBuffer, "accept form %s:%d\n", inet_ntoa(stClientAddr.sin_addr), stClientAddr.sin_port);
                printf("%d:%s", ++iAcceptCount, acBuffer);

                if (iCurFds >= MAXEPOLLSIZE)
                {
                    fprintf(stderr, "too many connection, more than %d\n", MAXEPOLLSIZE);
                    close(iConnFd);
                    continue;
                }
                
                if (SetFdNonBlocking(iConnFd) < 0)
                {
                    perror("setnonblocking error");
                }
                
                stEvent.events = EPOLLIN | EPOLLET;
                stEvent.data.fd = iConnFd;
                if (ControlEpoll(iEpollFd, EPOLL_CTL_ADD, iConnFd, &stEvent) < 0)
                {
                    fprintf(stderr, "add socket '%d' to epoll failed: %s\n", iConnFd, strerror(errno));
                    return -1;
                }
                
                iCurFds++;
                continue;
            }
            
            // 处理客户端请求
            if (EpollServerHandle(stEvents[iIndex].data.fd) < 0)
            {
                ControlEpoll(iEpollFd, EPOLL_CTL_DEL, stEvents[iIndex].data.fd, &stEvent);
                iCurFds--;
            }
        }
    }
    
    close(iListenFd);
    
    return 0;
}


