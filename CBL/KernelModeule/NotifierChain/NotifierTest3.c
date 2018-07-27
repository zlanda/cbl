/*******************************************************************************
*   文  件 名：NotifierTest3.c
*   功     能：实现测试内核通知链功能
*             发出通知链事件
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
 
extern int call_test_notifiers(unsigned long val, void *v);

#define TESTCHAIN_INIT                          0x52U
 
static int __init test_chain_2_init(void)
{
    printk(KERN_DEBUG "I'm in test_chain_2\n");
    
    call_test_notifiers(TESTCHAIN_INIT, "no_use");
    
    return 0;
}
 
static void __exit test_chain_2_exit(void)
{
    printk(KERN_DEBUG "Goodbye to test_chain_2\n");
}
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zlanda");
 
module_init(test_chain_2_init);
module_exit(test_chain_2_exit);


