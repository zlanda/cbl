/*******************************************************************************
*   文  件 名：WaitEventTest.c
*   功     能：测试内核wait event功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-28
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核wait event机制说明
	
*******************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static int condition;

static struct task_struct *tsk1;
static struct task_struct *tsk2;
static struct task_struct *tsk3;

int thread_one(void *p);
int thread_two(void *p);
int thread_third(void *p);

DECLARE_WAIT_QUEUE_HEAD(wq);

int thread_one(void *p)
{
	static int iExeCnt = 0;
	int i = 0;
	
	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

        if (iExeCnt > 1)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			while (i++ < 100)
			{
				wait_event(wq, condition == 1);
				msleep(1000);
				printk("thread_one run. recv event.\n");
			}

			iExeCnt++;
		}
	}

	return 0;
}

int thread_two(void *p)
{
	static int iExeCnt = 0;
	int i = 0;
	
	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

        if (iExeCnt > 1)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			while (i++ < 100) 
			{
				wait_event(wq, condition == 1);
				msleep(1000);
				printk("thread_two run. recv event.\n");
			}

			iExeCnt++;
		}
	}

	return 0;
}

int thread_third(void *p)
{
	static int iExeCnt = 0;
	int i = 0;

	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

		if (iExeCnt > 1)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			while (i++ < 10) 
			{
				condition = 0;
				msleep(2000);
				printk("thread_third run. trigger event.\n");
				condition = 1;
				wake_up(&wq);
				msleep(2000);
			}

			iExeCnt++;
		}
	}

	return 0;
}

static int waiteventtest_init(void)
{
	printk("spintest_init is coming\n");
	
	condition = 0;

	tsk1 = kthread_run(thread_one, NULL, "mythread%d", 1);
	if (IS_ERR(tsk1)) 
	{
		printk(KERN_INFO "create kthread tsk1 failed!\n");
	}
	else 
	{
		printk(KERN_INFO "create ktrhead tsk1 ok!\n");
	}
	
	tsk2 = kthread_run(thread_two, NULL, "mythread%d", 2);
	if (IS_ERR(tsk2)) 
	{
		printk(KERN_INFO "create kthread tsk2 failed!\n");
	}
	else 
	{
		printk(KERN_INFO "create ktrhead tsk2 ok!\n");
	}
	
	tsk3 = kthread_run(thread_third, NULL, "mythread%d", 3);
	if (IS_ERR(tsk3)) 
	{
		printk(KERN_INFO "create kthread tsk3 failed!\n");
	}
	else 
	{
		printk(KERN_INFO "create ktrhead tsk3 ok!\n");
	}
	
	return 0;
}

static void waiteventtest_exit(void)
{
	int ret;
	
	printk("\rspintest_exit exit\n");
	if (!IS_ERR(tsk1))
	{
		ret = kthread_stop(tsk1);
		printk(KERN_INFO "thread function exit %d\n", ret);
	}

	if (!IS_ERR(tsk2))
	{
		ret = kthread_stop(tsk2);
		printk(KERN_INFO "thread function exit %d\n", ret);
	}

	if (!IS_ERR(tsk3))
	{
		ret = kthread_stop(tsk3);
		printk(KERN_INFO "thread function exit %d\n", ret);
	}
	
	return;
}

MODULE_AUTHOR("zlanda");
MODULE_LICENSE("GPL");

module_init(waiteventtest_init);
module_exit(waiteventtest_exit);

