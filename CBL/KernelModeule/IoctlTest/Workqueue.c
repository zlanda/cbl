/*******************************************************************************
*   文  件 名：Workqueue.c
*   功     能：实现内核工作队列功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-26
*   修改历史：无
*******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <asm/atomic.h>

struct timer_data 
{
    struct timer_list timer;
    struct workqueue_struct *work_queue;
    unsigned long pre_jiffies;
    unsigned int loops;
};

/* 定义工作队列结构，实现参数传递 */
struct work_data
{
    unsigned long cnt;
    struct work_struct test_work;
};

static struct timer_data test_data;
static struct timer_data test_data1;

/* 定义工作队列 */
static struct work_data work_test_data;
static struct work_data work_test_data1;

static void do_work(struct work_struct *work);
//不需要传递参数时可以直接这样做
#if 0
static DECLARE_WORK(test_work, do_work);
static DECLARE_WORK(test_work1, do_work);
#endif

static struct workqueue_struct *test_workqueue;

atomic_t wq_run_times;
unsigned int failed_cnt = 0;

static int work_delay = 2 * HZ;

void test_timer_fn(unsigned long arg)
{
    struct timer_data *data = (struct timer_data *)arg;

    unsigned long j = jiffies;

    data->timer.expires += work_delay;
    data->pre_jiffies = j;
    mod_timer(&data->timer, jiffies + 2 * HZ);

    /* 调度前给出工作队列参数的值 */
    work_test_data.cnt++;

#if 0
    if (queue_work(test_workqueue, &test_work) == 0)
#endif
    //两种调度工作队列方式
#if 0
    if (queue_work(test_workqueue, &work_test_data.test_work) == 0)
#endif
    //也可以用schedule_work调度
    if (schedule_work(&work_test_data.test_work) == 0)
    {
        printk("Timer (0) add work queue failed\n");
        (*(&failed_cnt))++;
    }

    data->loops++;
    printk("timer-0 loops:%u\n", data->loops);
}

void test_timer_fn1(unsigned long arg)
{
    struct timer_data *data = (struct timer_data *)arg;

    unsigned long j = jiffies;

    data->timer.expires += 3 * HZ;
    data->pre_jiffies = j;
    mod_timer(&data->timer, jiffies + 3 * HZ);

    /* 调度前给出工作队列参数的值 */
    work_test_data1.cnt++;

#if 0
    if (queue_work(test_workqueue, &test_work1) == 0)
#endif
    //两种调度工作队列方式
#if 0
    if (queue_work(test_workqueue, &work_test_data1.test_work) == 0)
#endif
    if (schedule_work(&work_test_data1.test_work) == 0)
    {
        printk("Timer (1) add work queue failed\n");
        (*(&failed_cnt))++;
    }

    data->loops++;
    printk("timer-1 loops:%u\n", data->loops);
}

void do_work(struct work_struct *work)
{
    struct work_data *pstWorkData = NULL;
    
    atomic_inc(&wq_run_times);

    pstWorkData = container_of(work, struct work_data, test_work);

    printk("====work queue run times: %u====\n", atomic_read(&wq_run_times));

    printk("====failed count: %u====\n", *(&failed_cnt));

    printk("====work: %lu\n", pstWorkData->cnt);
}

int wq_init(void)
{
    unsigned long j = jiffies;
    printk("jiffies: %lu\n", jiffies);
    atomic_set(&wq_run_times, 0);

    init_timer(&test_data.timer);
    test_data.loops = 0;
    test_data.pre_jiffies = j;
    test_data.timer.function = test_timer_fn;
    test_data.timer.data = (unsigned long)(&test_data);
    test_data.timer.expires = j + 2 * HZ;
    add_timer(&test_data.timer);

    init_timer(&test_data1.timer);
    test_data1.loops = 0;
    test_data1.pre_jiffies = j;
    test_data1.timer.function = test_timer_fn1;
    test_data1.timer.data = (unsigned long)(&test_data1);
    test_data1.timer.expires = j + 3 * HZ;
    add_timer(&test_data1.timer);

    test_workqueue = create_singlethread_workqueue("test-wq");
    printk("test-wq init success.\n");
    printk("jiffies: %lu\n", jiffies);

    /* 带参数这里进行初始化 */
    /* 两种声明方式 */
    INIT_WORK(&work_test_data.test_work, do_work);
    INIT_WORK(&work_test_data1.test_work, do_work);

    return 0;
}

void wq_exit(void)
{
    del_timer(&test_data.timer);
    del_timer(&test_data1.timer);
    destroy_workqueue(test_workqueue);
    printk("wq exit success.\n");
}

