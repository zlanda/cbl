/*******************************************************************************
*   文  件 名：Timer.c
*   功     能：实现内核定时器功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-26
*   修改历史：无
*******************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/timer.h>
#include <linux/delay.h>

/* 定义定时器传递参数 */
struct timer_data
{
    unsigned long data;
    unsigned long jiffstime;
};

struct timer_list clk_timer;

struct timer_data clk_timer_data;

//定时器处理传递的是unsigned long类型的值，因此如果要传递结构体，则需要传递结构体的地址
void clk_timer_do(unsigned long l)
{
    struct timer_data *pstTimerData = (struct timer_data *)l;

    /* 修改data的值,查看下次定时器到期后其值是否被改变 */
    pstTimerData->data++;
    pstTimerData->jiffstime = jiffies + HZ;
    
    mod_timer(&clk_timer, jiffies + HZ);    //修改定时器过期时间
    printk(KERN_ALERT"param: %ld %ld, jiffies: %ld\n", pstTimerData->data, pstTimerData->jiffstime, jiffies);
}

void clk_timer_set(void)
{
    init_timer(&clk_timer);             //初始化定时器
    clk_timer.expires = jiffies + HZ;   //1s,过期时间
    clk_timer.function = clk_timer_do;  //设置定时器到期执行函数
    //clk_timer.data = 9527;            //定时器执行函数传递参数，可以定义结构体传输
    clk_timer_data.data = 9527;
    clk_timer_data.jiffstime = jiffies;
    clk_timer.data = (unsigned long)&clk_timer_data;
    add_timer(&clk_timer);              //添加定时器到内核
}

int clk_init(void)
{
    printk(KERN_ALERT"===%s===\n", __func__);
    clk_timer_set();                    //设置定时器
    return 0;
}

void clk_exit(void)
{
    printk(KERN_ALERT"===%s===\n", __func__);
    del_timer(&clk_timer);              //删除定时器
}

