#include "Epoll.h"

#define MAXLINE 1024

VOID EpollClientHandle(INT32 iSockFd)
{
    CHAR acSendLine[MAXLINE];
    CHAR acRecvLine[MAXLINE];
    INT32 iNum;
    for (;;) 
    {
        if (fgets(acSendLine, MAXLINE, stdin) == NULL)
        {
            break;//read eof
        }
        
        /*
        //也可以不用标准库的缓冲流,直接使用系统函数无缓存操作
        if (read(STDIN_FILENO, sendline, MAXLINE) == 0)
        {
            break;//read eof
        }
        */

        iNum = write(iSockFd, acSendLine, strlen(acSendLine));
        iNum = read(iSockFd, acRecvLine, MAXLINE);
        if (iNum == 0)
        {
            printf("echoclient: server terminated prematurely\n");
            break;
        }
        
        write(STDOUT_FILENO, acRecvLine, iNum);
        //如果用标准库的缓存流输出有时会出现问题
        //fputs(recvline, stdout);
    }

    return;
}

int main(int argc, char **argv)
{
    CHAR *pcServInetAddr = "127.0.0.1";
    INT32 iServPort = 6888;
    INT32 iConnFd;
    struct sockaddr_in stServAddr;

    if (argc == 2)
    {
        pcServInetAddr = argv[1];
    }
    
    if (argc == 3)
    {
        pcServInetAddr = argv[1];
        iServPort = atoi(argv[2]);
    }
    
    if (argc > 3)
    {
        printf("usage: echoclient <IPaddress> <Port>\n");
        return -1;
    }

    iConnFd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&stServAddr, sizeof(stServAddr));
    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(iServPort);
    inet_pton(AF_INET, pcServInetAddr, &stServAddr.sin_addr);

    if (connect(iConnFd, (struct sockaddr *) &stServAddr, sizeof(stServAddr)) < 0)
    {
        perror("connect error");
        return -1;
    }
    
    printf("welcome to echoclient\n");
    EpollClientHandle(iConnFd);     /* do it all */
    close(iConnFd);
    printf("exit\n");
    
    return 0;
}

