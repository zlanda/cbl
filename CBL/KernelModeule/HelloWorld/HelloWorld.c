/*******************************************************************************
*   文  件 名：HelloWorld.c
*   功     能：实现内核HelloWorld功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>

static int helloworld_init(void) 
{
    printk(KERN_INFO "hello world!\n");

    return 0;
}

static int helloworld_exit(void) 
{
    printk(KERN_INFO "see you.\n");

    return 0;
}

module_init(helloworld_init);
module_exit(helloworld_exit);

