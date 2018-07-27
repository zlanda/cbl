/*******************************************************************************
*   文  件 名：NotifierTest1.c
*   功     能：实现测试内核通知链功能
*             0. 申明一个通知链；
*             1. 向内核注册通知链；
*             2. 定义事件；
*             3. 导出符号，因而必需最后退出
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-27
*   修改历史：无
*******************************************************************************/
#include <linux/notifier.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/fs.h>       /* everything() */

/*******************************************************************************
                                   内核通知链机制说明
    Linux内核中各个子系统相互依赖，当其中某个子系统状态发生改变时，就必须使用一定
的机制告知使用其服务的其他子系统，以便其他子系统采取相应的措施。为满足这样的需求，
内核实现了事件通知链机制（notificationchain）。
    通知链只能用在各个子系统之间，而不能在内核和用户空间进行事件的通知。组成内核
的核心系统代码均位于kernel目录下，通知链表位于kernel/notifier.c中，对应的头文件为
include/linux/notifier.h。通知链表机制并不复杂，实现它的代码只有区区几百行。
    事件通知链表是一个事件处理函数的列表，每个通知链都与某个或某些事件有关，当特定
的事件发生时，就调用相应的事件通知链中的回调函数，进行相应的处理。

    Linux的网络子系统一共有3个通知链：表示ipv4地址发生变化时的inetaddr_chain；表示
ipv6地址发生变化的inet6addr_chain；还有表示设备注册、状态变化的netdev_chain。
    在这些链中都是一个个notifier_block结构：
    struct notifier_block {
        int (*notifier_call)(struct notifier_block *, unsigned long, void *);
        struct notifier_block *next;
        int priority;
    };
    其中
    1、notifier_call：当相应事件发生时应该调用的函数，由被通知方提供，如other_subsys_1；
    2、notifier_block *next：用于链接成链表的指针；
    3、priority：回调函数的优先级，一般默认为0。
    
    内核代码中一般把通知链命名为xxx_chain, xxx_nofitier_chain这种形式的变量名。围绕核心
数据结构notifier_block，内核定义了四种通知链类型：
    1、原子通知链（ Atomic notifier chains ）：通知链元素的回调函数（当事件发生时要执行
的函数）在中断或原子操作上下文中运行，不允许阻塞。对应的链表头结构：
    struct atomic_notifier_head {
        spinlock_t  lock;
        struct  notifier_block *head;
    };
    2、可阻塞通知链（ Blocking notifier chains ）：通知链元素的回调函数在进程上下文中运行，
允许阻塞。对应的链表头：
    struct  blocking_notifier_head {
        struct  rw_semaphore  rwsem;
        struct  notifier_block   *head;
    };
    3、原始通知链（ Raw notifierchains ）：对通知链元素的回调函数没有任何限制，所有锁和保护
机制都由调用者维护。对应的链表头：
    网络子系统就是该类型，通过以下宏实现head的初始化
    static RAW_NOTIFIER_HEAD(netdev_chain);
#define RAW_NOTIFIER_INIT(name)     {      \
        .head= NULL }
#define RAW_NOTIFIER_HEAD(name)         \      //调用他就好了
        struct raw_notifier_head name =         \
        RAW_NOTIFIER_INIT(name)   
    即：
    struct raw_notifier_head netdev_chain = {
        .head = NULL;
    }
    而其回调函数的注册，比如向netdev_chain的注册函数：register_netdevice_notifier。
    struct  raw_notifier_head {
        struct  notifier_block   *head;
    };
    4、SRCU 通知链（ SRCU notifier chains ）：可阻塞通知链的一种变体。对应的链表头：
    struct  srcu_notifier_head {
        struct  mutex mutex;
        struct  srcu_struct  srcu;
        struct  notifier_block  *head;
    };

    运行机制：
    被通知一方（other_subsys_x）通过notifier_chain_register向特定的chain注册回调函数，
并且一般而言特定的子系统会用特定的notifier_chain_register包装函数来注册，比如路由子系统
使用的是网络子系统的：register_netdevice_notifier来注册他的notifier_block。
    1、向事件通知链注册的步骤
        a、申明struct notifier_block结构
        b、编写notifier_call函数
        c、调用特定的事件通知链的注册函数，将notifier_block注册到通知链中
    如果内核组件需要处理某个事件通知链上发出的事件通知，其就该在初始化时
    在该通知链上注册回调函数。
    2、通知子系统有事件发生
        inet_subsys是通过notifier_call_chain来通知其他的子系统（other_subsys_x）的。
        notifier_call_chain会按照通知链上各成员的优先级顺序执行回调函数（notifier_call_x）；
    回调函数的执行现场在notifier_call_chain进程地址空间；其返回值是NOTIFY_XXX的形式，
    在include/linux/notifier.h中：
        #define NOTIFY_DONE            0x0000         // 对事件视而不见
        #define NOTIFY_OK              0x0001         // 事件正确处理
        #define NOTIFY_STOP_MASK  0x8000              // 由notifier_call_chain检查，看继续调用回调函数，还是停止，
                                                      // _BAD和_STOP中包含该标志
        #define NOTIFY_BAD        (NOTIFY_STOP_MASK|0x0002)   // 事件处理出错，不再继续调用回调函数
        //
        //Clean way to return from the notifier and stop further calls.
        //
        #define NOTIFY_STOP             (NOTIFY_OK|NOTIFY_STOP_MASK)    // 回调出错，不再继续调用该事件回调函数

        notifier_call_chain捕获并返回最后一个事件处理函数的返回值；注意：notifier_call_chain
    可能同时被不同的cpu调用，故而调用者必须保证互斥。
    3、事件列表
        对于网络子系统而言，其事件常以NETDEV_XXX命名；描述了网络设备状态（dev->flags）、
    传送队列状态（dev->state）、设备注册状态（dev->reg_state），以及设备的硬件功能特性
    （dev->features）：
        include/linux/notifier.h中
        // netdevice notifier chain
        #define NETDEV_UP  0x0001  // 激活一个网络设备
        #define NETDEV_DOWN  0x0002f // 停止一个网络设备，所有对该设备的引用都应释放
        #define NETDEV_REBOOT       0x0003       // 检查到网络设备接口硬件崩溃，硬件重启
        #define NETDEV_CHANGE       0x0004  // 网络设备的数据包队列状态发生改变
        #define NETDEV_REGISTER  0x0005   // 一个网络设备事例注册到系统中，但尚未激活
        #define NETDEV_UNREGISTER       0x0006       // 网络设备驱动已卸载
        #define NETDEV_CHANGEMTU      0x0007  // MTU发生了改变
        #define NETDEV_CHANGEADDR    0x0008  // 硬件地址发生了改变
        #define NETDEV_GOING_DOWN   0x0009  // 网络设备即将注销，有dev->close报告，通知相关子系统处理
        #define NETDEV_CHANGENAME   0x000A  // 网络设备名改变
        #define NETDEV_FEAT_CHANGE    0x000B  //feature网络硬件功能改变
        #define NETDEV_BONDING_FAILOVER 0x000C  //    
        #define NETDEV_PRE_UP        0x000D  //    
        #define NETDEV_BONDING_OLDTYPE  0x000E              //   
        #define NETDEV_BONDING_NEWTYPE  0x000F      //    
*******************************************************************************/
     
#define TESTCHAIN_INIT                          0x52U

static RAW_NOTIFIER_HEAD(test_chain);

/* define our own notifier_call_chain */
static int call_test_notifiers(unsigned long val, void *v)
{
    return raw_notifier_call_chain(&test_chain, val, v);
}

EXPORT_SYMBOL(call_test_notifiers);

/* define our own notifier_chain_register func */
static int register_test_notifier(struct notifier_block *nb)
{
    int err;
    
    err = raw_notifier_chain_register(&test_chain, nb);
    if(err)
    {
        printk("raw_notifier_chain_register failed.\r\n");
        goto out;
    }

out:
    return err;
}

EXPORT_SYMBOL(register_test_notifier);

static int __init test_chain_0_init(void)
{
    printk(KERN_DEBUG "I'm in test_chain_0\n");

    return 0;
}

static void __exit test_chain_0_exit(void)
{
    printk(KERN_DEBUG "Goodbye to test_chain_0\n");
    //  call_test_notifiers(TESTCHAIN_EXIT, (int *)NULL);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zlanda");

module_init(test_chain_0_init);
module_exit(test_chain_0_exit);

