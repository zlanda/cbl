#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>

// 自定义的协议号
#define NETLINK_TEST                    31

// 消息类型
#define NLMSG_SETECHO                   0x11
#define NLMSG_GETECHO                   0x12

// 最大协议负荷(固定)
#define MAX_PAYLOAD                     101

int main(int argc, char **argv)
{
    int iSockFd = -1;
    struct iovec stIov;
    struct sockaddr_nl stSrcAddr;
    struct sockaddr_nl stDstAddr;
    struct nlmsghdr *pstNlHeader = NULL;
    struct msghdr stMsgHeader;
    
    if (argc != 2)
    {
        printf("usage: ./a.out <str>\n");
        exit(-1);
    }

    /* 创建NETLINK_TEST协议的socket */
    iSockFd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_TEST);
    
    /* 设置本地端点并绑定，用于侦听 */
    bzero(&stSrcAddr, sizeof(stSrcAddr));
    stSrcAddr.nl_family = AF_NETLINK;
    stSrcAddr.nl_pid = getpid();
    stSrcAddr.nl_groups = 0; //未加入多播组
    bind(iSockFd, (struct sockaddr*)&stSrcAddr, sizeof(stSrcAddr));
    
    /* 构造目的端点，用于发送 */
    bzero(&stDstAddr, sizeof(stDstAddr));
    stDstAddr.nl_family = AF_NETLINK;
    stDstAddr.nl_pid = 0; // 表示内核
    stDstAddr.nl_groups = 0; //未指定接收多播组
    
    /* 构造发送消息 */
    pstNlHeader = malloc(NLMSG_SPACE(MAX_PAYLOAD));
    pstNlHeader->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD); //保证对齐
    pstNlHeader->nlmsg_pid = getpid();  /* self pid */
    pstNlHeader->nlmsg_flags = 0;
    pstNlHeader->nlmsg_type = NLMSG_GETECHO;
    strcpy(NLMSG_DATA(pstNlHeader), argv[1]);
    
    stIov.iov_base = (void *)pstNlHeader;
    stIov.iov_len = pstNlHeader->nlmsg_len;
    
    stMsgHeader.msg_name = (void *)&stDstAddr;
    stMsgHeader.msg_namelen = sizeof(stDstAddr);
    stMsgHeader.msg_iov = &stIov;
    stMsgHeader.msg_iovlen = 1;

    /* 发送消息到内核 */
    sendmsg(iSockFd, &stMsgHeader, 0);
    
    /* 接收消息 */
    memset(pstNlHeader, 0, NLMSG_SPACE(MAX_PAYLOAD));
    recvmsg(iSockFd, &stMsgHeader, 0);

    /* 打印消息 */
    printf(" Received message payload: %s\n", NLMSG_DATA(pstNlHeader));
    
    return 0;
}

