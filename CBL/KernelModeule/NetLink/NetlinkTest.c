/*******************************************************************************
*   文  件 名：NetlinkTes.c
*   功     能：测试内核Netlink功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-30
*   修改历史：无
*******************************************************************************/

//https://blog.csdn.net/balance156/article/details/15372831

#include <linux/module.h>
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/net_namespace.h>

#define NETLINK_TEST                        31
#define NLMSG_SETECHO                       0x11
#define NLMSG_GETECHO                       0x12
      
static struct sock *pstSock;

static void nl_custom_data_ready(struct sk_buff *skb)
{
    struct nlmsghdr *pstNlHeader;
    void *pvPayload;
    struct sk_buff *pstOutSkb;
    void *pvOutPayload;
    struct nlmsghdr *pstOutNlHeader;
    int iPayloadLen; // with padding, but ok for echo
    
    pstNlHeader = nlmsg_hdr(skb);
    switch (pstNlHeader->nlmsg_type)
    {
        case NLMSG_SETECHO:
        {
            break;
        }
            
        case NLMSG_GETECHO:
        {
            pvPayload = nlmsg_data(pstNlHeader);
            iPayloadLen = nlmsg_len(pstNlHeader);
            
            printk(KERN_INFO "payload_len = %d\n", iPayloadLen);
            printk(KERN_INFO "Recievid: %s, From: %d\n", (char *)pvPayload, pstNlHeader->nlmsg_pid);

            //分配足以存放默认大小的sk_buff
            pstOutSkb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL); 
            if (!pstOutSkb)
            {
                goto failure;
            }

            //填充协议头数据
            pstOutNlHeader = nlmsg_put(pstOutSkb, 0, 0, NLMSG_SETECHO, iPayloadLen, 0); 
            if (!pstOutNlHeader)
            {
                goto failure;
            }
            
            pvOutPayload = nlmsg_data(pstOutNlHeader);
            // 在响应中加入字符串，以示区别
            strcpy(pvOutPayload, "[from kernel]:"); 
            strcat(pvOutPayload, pvPayload);
            nlmsg_unicast(pstSock, pstOutSkb, pstNlHeader->nlmsg_pid);
            
            break;
        }
            
        default:
        {
            printk(KERN_INFO "Unknow msgtype recieved!\n");
            
            return;
        }
    }
    
    return;
    
failure:
    printk(KERN_INFO " failed in fun dataready!\n");

    return;
}

int __init netlink_test_init(void)
{
    struct netlink_kernel_cfg stNlCfg = 
    {
        .input = nl_custom_data_ready,
    };
    
    pstSock = netlink_kernel_create(&init_net, NETLINK_TEST, &stNlCfg);
    printk(KERN_INFO "initialed ok!\n");
    
    if (!pstSock) 
    {
        printk(KERN_INFO "netlink create error!\n");
    }
    
    return 0;
}

void __exit netlink_test_exit(void)
{
    printk(KERN_INFO "existing...\n");
    netlink_kernel_release(pstSock);

    return;
}

module_init(netlink_test_init);
module_exit(netlink_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("netlink test");
MODULE_AUTHOR("zlandas");

