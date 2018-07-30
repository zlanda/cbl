#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <netinet/ether.h>
#include <string.h>

/*******************************************************************************
                                原始套接字说明
    原始套接字(SOCK_RAW)是一种不同于SOCK_STREAM、SOCK_DGRAM的套接字，它实现与系统
核心。然而，原始套接字能做什么呢？首先来说，普通的套接字无法处理ICMP、IGMP等网络报
文，而SOCK_RAW可以；其次，SOCK_RAW也可以处理特殊的IPv4报文；此外，利用原始套接字，
可以通过IP_HDRINCL套接字选项由用户构造IP头。总体来说，SOCK_RAW可以处理普通的网络报
文之外，还可以处理一些特殊协议报文以及操作IP层及其以上的数据。
    既然SOCK_RAW有以上特性，所以在某些处理流程上它区别于普通套接字。
        1、若设置IP_HDRINCL选项，SOCK_RAW可以操作IP头数据（也就是用户需用填充IP头
            及其以上的payload）；否则SOCK_RAW无法操作IP头数据
        2、端口对于SOCK_RAW而言没有任何意义
        3、如果使用bind函数绑定本地IP，那么如果IP_HDRINCL未设置，则用此IP填充源IP
            地址；若不调用bind则将源IP地址设置为外出接口的主IP地址
        4、如果使用connect函数设置目标IP，则可以使用send或者write函数发送报文，而不
            需要使用sendto函数

    内核处理流程：
        1、接收到的TCP、UDP分组不会传递给任何SOCK_RAW
        2、ICMP、IGMP报文分组传递给SOCK_RAW
        3、内核不识别的IP报文传递给SOCK_RAW

    SOCK_RAW是否接收报文：
        1、Protocol指定类型需要匹配，否则不传递给该SOCK_RAW
        2、如果使用bind函数绑定了源IP，则报文目的IP必须和绑定的IP匹配，否则不传递给该SOCK_RAW
        3、如果使用connect函数绑定了目的IP，则报文源IP必须和指定的IP匹配，否则不传递给该SOCK_RAW 

    综上所述，原始套接字处理的只是IP层及其以上的数据，比如实现SYN FLOOD攻击、处理PING报文等。
当需要操作更底层的数据的时候，就需要采用其他的方式。

    链路层处理报文：
    如果需要从链路层处理报文，那么就需要采用更加底层的套接字。还是先看下套接字函数的原型吧：
    #include <sys/socket.h>
    int socket(int domain, int type, int protocol);
    这个函数中，domain表示协议簇，type表示套接字类型，而protocol表示的是处理的协议类型。
在Linux下提供了多种底层套接字。下面分别进行简单介绍。
    1、PF_INET协议簇
        通过PF_INET可以构造原始套接字，如下所示：
        int fd = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
        正如前面所讲的，它工作在IP层及其以上各层协议上（当然是在使用IP_HDRINCL选项之后才能
    操作IP层数据啦），但是这种套接字无法接收从本地发送出去的报文。而使用SOCK_PACKET类型的套
    接字，则可以操作链路层数据了：
        int fd = socket (PF_INET, SOCK_PACKET, IPPROTO_TCP);
        不过据说这种方式存在一定的缺陷，而且也不能保证后续版本的系统上一定支持这种方式，因此
    不推荐使用
    2、PF_PACKET协议簇
        PF_PACKET协议簇是用来取代SOCK_PACKET的一种编程接口。作为一种协议簇，它可以对应两种
    不同的套接字类型：SOCK_RAW和SOCK_DGRAM。当使用SOCK_RAW时，用户操作链路层数据，但是如果
    使用后者，则由系统处理链路层协议头。这种套接字支持四种协议（ETH_P_IP、ETH_P_ARP、ETH_P_RARP、
    ETH_P_ALL）（未确认）
        int fd = socket (PF_PACKET, SOCK_RAW, IPPROTO_TCP);
        int fd = socket (PF_PACKET, SOCK_DGRAM, IPPROTO_TCP);
    3、NETLINK协议簇
        这种方式是用户模式和kernel的IP网络配置之间的推荐接口

    综上所述，真正能够实现操作链路层数据的只有三种方式：
        int fd = socket (PF_INET, SOCK_PACKET, IPPROTO_TCP);
        int fd = socket (PF_PACKET, SOCK_RAW, IPPROTO_TCP);
        int fd = socket (PF_PACKET, SOCK_DGRAM, IPPROTO_TCP);

    sock_raw使用：
    （注意一定要在root下使用）原始套接字编程可以接收到本机网卡上的数据帧或者数据包,对与监听
网络的流量和分析是很有作用的.一共可以有3种方式创建这种socket
    1、socket(AF_INET, SOCK_RAW, IPPROTO_TCP|IPPROTO_UDP|IPPROTO_ICMP)发送接收ip数据包，
        不能用IPPROTO_IP，因为如果是用了IPPROTO_IP，系统根本就不知道该用什么协议。
    2、socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP|ETH_P_ARP|ETH_P_ALL))发送接收以太网数据帧
    3、socket(AF_INET, SOCK_PACKET, htons(ETH_P_IP|ETH_P_ARP|ETH_P_ALL))过时了,不要用啊


    1、socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    该套接字可以接收协议类型为(tcp udp icmp等)发往本机的ip数据包；
    不能收到非发往本地ip的数据包(ip软过滤会丢弃这些不是发往本机ip的数据包)；
    不能收到从本机发送出去的数据包；
    发送的话需要自己组织tcp udp icmp等头部.可以setsockopt来自己包装ip头部。

    2、socket(PF_PACKET, SOCK_RAW, htons(x));
    这个套接字比较强大,创建这种套接字可以监听网卡上的所有数据帧。最后一个以太网crc
从来都不算进来的,因为内核已经判断过了,对程序来说没有任何意义了.
    接收发往本地mac的数据帧；
    接收从本机发送出去的数据帧(第3个参数需要设置为ETH_P_ALL)；
    接收非发往本地mac的数据帧(网卡需要设置为promisc混杂模式)；

    协议类型一共有四个：
    ETH_P_IP 0x800 只接收发往本机mac的ip类型的数据帧；
    ETH_P_ARP 0x806 只接受发往本机mac的arp类型的数据帧；
    ETH_P_RARP 0x8035 只接受发往本机mac的rarp类型的数据帧；
    ETH_P_ALL 0x3 接收发往本机mac的所有类型ip arp rarp的数据帧, 接收从本机发出的
所有类型的数据帧.(混杂模式打开的情况下,会接收到非发往本地mac的数据帧)

    发送的时候需要自己组织整个以太网数据帧.所有相关的地址使用struct sockaddr_ll
而不是struct sockaddr_in(因为协议簇是PF_PACKET不是AF_INET了),比如发送给某个机器,
对方的地址需要使用struct sockaddr_ll.
    
*******************************************************************************/

/* 定义接收缓冲区大小 */
#define BUFFER_MAX_LEN                      1024

//子函数，用于显示源目的IP地址
void showip(unsigned char *pcBuffer)
{
    /* IP头指针 */
    struct iphdr *pstIpHeader;
    //源IP地址
    char acSourIpAddr[INET_ADDRSTRLEN] = "";
    //目的IP地址
    char acDestIpAddr[INET_ADDRSTRLEN] = "";

    //指针接收报文的IP头
    pstIpHeader = (struct iphdr*)(pcBuffer + sizeof(struct ethhdr));
    //获取源IP地址
    inet_ntop(AF_INET, &pstIpHeader->saddr, acSourIpAddr, INET_ADDRSTRLEN);
    //获取目的IP地址
    inet_ntop(AF_INET, &pstIpHeader->daddr, acDestIpAddr, INET_ADDRSTRLEN);

    //显示IP地址
    printf("IP: %s >> %s\n", acSourIpAddr, acDestIpAddr);

    return;
}

//主函数
int main(int argc, char **argv)
{
	int iSockFd;
	int iRecvLen;
	unsigned char acRecvBuffer[BUFFER_MAX_LEN];
	struct ethhdr *pstEthHeader;
	char acPrtoType[5] = "";
	uint16_t *pusPort = NULL;

	//创建原始套接字
	if ((iSockFd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		perror("socket");
		exit(1);
	}

	//接收数据包
	while (1)
	{
		bzero(acRecvBuffer, sizeof(acRecvBuffer));
		//接收数据包
		iRecvLen = recvfrom(iSockFd, acRecvBuffer, BUFFER_MAX_LEN, 0, NULL, NULL);
		//分析数据包类型
		sprintf(acPrtoType, "%02x%02x", acRecvBuffer[12], acRecvBuffer[13]);

		//0800IP数据包，接收到IP数据包
		if (0 == strcmp(acPrtoType, "0800"))
		{
			printf("============IP===============\n");
			//TCP还是UDP
			if (0x06 == acRecvBuffer[23])
			{
				printf("TCP\n");
			}
			else if (0x11 == acRecvBuffer[23])
			{
				printf("UDP\n");
			}

			//源端口号
			pusPort = acRecvBuffer + 34;
			printf("Port:%d >> ", ntohs(*pusPort));
			//目的端口号
			pusPort = acRecvBuffer + 36;
			printf("%d\n", ntohs(*pusPort));

			//显示IP地址
			showip(acRecvBuffer);
		}
		//接收到ARP报文
		else if (0 == strcmp(acPrtoType,"0806"))
		{
			printf("============ARP===============\n");
			showip(acRecvBuffer);
		}
		//接收到RARP报文
		else if (0 == strcmp(acPrtoType,"8035"))
		{
			printf("============RARP===============\n");
		}
		//接收到其他协议类型报文
		else
		{
			printf("============%s===============\n", acPrtoType);
		}

        //获取链路层头指针		
		pstEthHeader = (struct ethhdr*)acRecvBuffer;
		
		//显示MAC地址
		printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X >> %02X:%02X:%02X:%02X:%02X:%02X \n",
		        pstEthHeader->h_source[0], pstEthHeader->h_source[1], pstEthHeader->h_source[2], 
		        pstEthHeader->h_source[3], pstEthHeader->h_source[4], pstEthHeader->h_source[5], 
		        pstEthHeader->h_dest[0], pstEthHeader->h_dest[1], pstEthHeader->h_dest[2],
		        pstEthHeader->h_dest[3], pstEthHeader->h_dest[4], pstEthHeader->h_dest[5]);		
	}

	//关闭socket
	close(iSockFd);

    return 0;
}

