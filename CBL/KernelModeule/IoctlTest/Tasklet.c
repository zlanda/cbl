/*******************************************************************************
*   文  件 名：Tasklet.c
*   功     能：实现内核Tasklet功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-26
*   修改历史：无
*******************************************************************************/
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/fs.h> 
#include <linux/interrupt.h> 

struct timer_data 
{
    struct timer_list timer;
    unsigned int loops;
};

static struct tasklet_struct my_tasklet;

static struct timer_data test_data;

//tasklet处理函数  
static void tasklet_handler(unsigned long data)  
{
    struct timer_data *timer_data = (struct timer_data *)data;
    
    printk(KERN_ALERT "tasklet_handler is running loop: %d.\n", timer_data->loops);
}

static void tasklet_timer_fn(unsigned long arg)
{
    struct timer_data *data = (struct timer_data *)arg;

    data->loops++;
    mod_timer(&data->timer, jiffies + 2 * HZ);

    //调度tasklet处理程序  
    tasklet_schedule(&my_tasklet);
}

int tasklet_demo_init(void)
{
    unsigned long tasklet_data;
    unsigned long j = jiffies;
    
    printk("jiffies: %lu\n", jiffies);

    init_timer(&test_data.timer);
    test_data.loops = 0;
    test_data.timer.function = tasklet_timer_fn;
    test_data.timer.data = (unsigned long)(&test_data);
    test_data.timer.expires = j + 2 * HZ;
    add_timer(&test_data.timer);
    
    //初始化tasklet
    /*
        void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long), unsigned long data)
    */
    //将定时器的数据作为参数传递到tasklet
    tasklet_data = (unsigned long)(&test_data);
    tasklet_init(&my_tasklet, tasklet_handler, tasklet_data);  
       
    printk(KERN_ALERT "tasklet_init.\n");  
    return 0;  
}

void tasklet_demo_exit(void)  
{
    del_timer(&test_data.timer);
    
    //  销毁tasklet  
    tasklet_kill(&my_tasklet);  
    printk(KERN_ALERT "tasklet_exit.\n");  
}  

