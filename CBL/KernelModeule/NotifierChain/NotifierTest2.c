/*******************************************************************************
*   文  件 名：NotifierTest2.c
*   功     能：实现测试内核通知链功能
*             1. 定义回调函数；
*             2. 定义notifier_block；
*             3. 向chain_0注册notifier_block；
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
 
extern int register_test_notifier(struct notifier_block *nb);

#define TESTCHAIN_INIT                          0x52U
 
/* realize the notifier_call func */
int test_init_event(struct notifier_block *nb, unsigned long event, void *v)
{
    switch(event)
    {
        case TESTCHAIN_INIT:
            printk(KERN_DEBUG "I got the chain event: test_chain_2 is on the way of init\n");
            break;
     
        default:
            break;
    }
 
    return NOTIFY_DONE;
}

/* define a notifier_block */
static struct notifier_block test_init_notifier = 
{
    .notifier_call = test_init_event,
};

static int __init test_chain_1_init(void)
{
    printk(KERN_DEBUG "I'm in test_chain_1\n");
    
    register_test_notifier(&test_init_notifier);    // 由chain_0提供的设施

    return 0;
}
 
static void __exit test_chain_1_exit(void)
{
    printk(KERN_DEBUG "Goodbye to test_clain_l\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zlanda");
 
module_init(test_chain_1_init);
module_exit(test_chain_1_exit);

