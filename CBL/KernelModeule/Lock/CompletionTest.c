/*******************************************************************************
*   文  件 名：CompletionTest.c
*   功     能：测试内核完成量功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-29
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核完成量机制说明
	完成量(completion)是Linux系统提供的一种比信号量更好的同步机制,是对信号量的一种补充;
它用于一个执行单元等待另一个执行单元完成某事;使用完成量等待时,调用进程是以独占睡眠方式
进行等待的;不是忙等待;
	1、定义完成量:
		struct completion my_completion;    //定义完成量my_completion
	2、初始化完成量:
		init_completion(&my_completion);    //初始化完成量my_completion
		DECLARE_COMPLETION(my_completion);  //初始化完成量my_completion的快捷方式
	3、等待完成量:
		void wait_for_completion(struct completion* comp):
		该函数等待一个完成量被唤醒;该函数会阻塞调用进程,如果所等待的完成量没有被唤醒,那就
	一直阻塞下去,而且不会被信号打断;
		int wait_for_completion_interruptible(struct completion* comp):
		该函数等待一个完成量被唤醒;但是它可以被外部信号打断;
		int wait_for_completion_killable(struct completion* comp):
		该函数等待一个完成量被唤醒;但是它可以被kill信号打断;
		unsigned long wait_for_completion_timeout(struct completion* comp, unsigned long timeout):
		该函数等待一个完成量被唤醒;该函数会阻塞调用进程,如果所等待的完成量没有被唤醒,调用
	进程也不会一直阻塞下去,而是等待一个指定的超时时间timeout,当超时时间到达时,如果所等待的
	完成量仍然没有被唤醒,那就返回;超时时间timeout以系统的时钟滴答次数jiffies计算;
		bool try_wait_for_completion(struct completion* comp):
		该函数尝试等待一个完成量被唤醒;不管所等待的完成量是否被唤醒,该函数都会立即返回;
		bool completion_done(struct completion* comp):
		该函数用于检查是否有执行单元阻塞在完成量comp上(是否已经完成),返回0,表示有执行单元
	被完成量comp阻塞;相当于wait_for_completion_timeout()中的timeout=0;
	
		注意:这几个函数都会把进程添加到等待队列中,进程在等待队列中以独占睡眠方式进行等待,
	直到请求被内核中的某些部分处理;
	4、唤醒完成量:
		void complete(struct completion* comp):
		该函数只唤醒一个正在等待完成量comp的执行单元;
		void complete_all(struct completion* comp):
		该函数唤醒所有正在等待同一个完成量comp的执行单元;
		NORET_TYPE void complete_and_exit(struct completion* comp, long code):
		该函数唤醒一个正在等待完成量comp的执行单元,并退出,code为退出码;
		
		注意:在内核处理完请求之后,必须调用这三个函数中的一个,来唤醒其它正在等待的进程;


	wait event与completion之间的区别：
	completion机制是两个进程间互相同步的工具，而wait_for_event则是让多个进程在一个等待队列
上休眠等待某事件的机制。
	completion以一个completion结构体变量为核心，两个进程围绕该变量执行wait_for_completion和complete操作，
以达到实现两个进程同步的目的；
	wait_for_event则是一个一个等待队列为核心，N个进程围绕该队列执行wait_event(将自己加入等待队列)和
wake_up(唤醒队列所有等待进程检查唤醒条件是否满足)操作，以达到使进程在等待条件未满足时保持休眠的目的。

*******************************************************************************/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/err.h>
#include <linux/completion.h>

//static int sleep_time = (1*10*HZ);
static int shared_res = 0;
struct completion my_comp;

static struct task_struct *tsk1;
static struct task_struct *tsk2;
static struct task_struct *tsk3;
static struct task_struct *tsk4;

int thread_one(void *p);
int thread_two(void *p);
int thread_third(void *p);
int thread_forth(void *p);

int thread_one(void *p)
{
	while(1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if(kthread_should_stop())
		{
			printk("kernel thread '%s' should stop;file:%s;line:%d\n", __FUNCTION__, __FILE__, __LINE__);
			break;
		}

		//对临界区加锁
		wait_for_completion(&my_comp);
		shared_res++;
		//对临界区解锁
		complete(&my_comp);

		//mdelay(sleep_time);
		schedule_timeout(HZ);
	}
	
	return 12;
}

int thread_two(void *p)
{
	while(1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if(kthread_should_stop())
		{
			printk("kernel thread '%s' should stop;file:%s;line:%d\n", __FUNCTION__, __FILE__, __LINE__);
			break;
		}

		//对临界区加锁
		wait_for_completion(&my_comp);
		shared_res++;
		//对临界区解锁
		complete(&my_comp);

		//msleep(sleep_time);
		schedule_timeout(HZ);
	}
	return 34;
}

int thread_third(void *p)
{
	int val = 0;

	while(1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if(kthread_should_stop())
		{
			printk("kernel thread '%s' should stop;file:%s;line:%d\n", __FUNCTION__, __FILE__, __LINE__);
			break;
		}

		//对临界区加锁
		wait_for_completion(&my_comp);
		val = shared_res;
		printk("%s: shared resource = %d;\n%s", __FUNCTION__, val, ((val % 3) ? "" : "\n"));
		//对临界区解锁
		complete(&my_comp);

		//msleep(sleep_time);
		schedule_timeout(HZ);
	}
	
	return 56;
}

int thread_forth(void *p)
{
	int val = 0;

	while(1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if(kthread_should_stop())
		{
			printk("kernel thread '%s' should stop;file:%s;line:%d\n", __FUNCTION__, __FILE__, __LINE__);
			break;
		}

		//对临界区加锁
		wait_for_completion(&my_comp);
		val = shared_res;
		printk("%s: shared resource = %d;\n%s", __FUNCTION__, val, ((val % 3) ? "" : "\n"));
		//对临界区解锁
		complete(&my_comp);

		//msleep(sleep_time);
		schedule_timeout(HZ);
	}
	
	return 78;
}

static int completiontest_init(void)
{
	printk("completiontest_init is coming\n");
	
	init_completion(&my_comp);
	printk("init completion ok\n");
	complete_all(&my_comp);

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

	tsk4 = kthread_run(thread_forth, NULL, "mythread%d", 4);
	if (IS_ERR(tsk4)) 
	{
		printk(KERN_INFO "create kthread tsk4 failed!\n");
	}
	else 
	{
		printk(KERN_INFO "create ktrhead tsk4 ok!\n");
	}
	
	return 0;
}

static void completiontest_exit(void)
{
	int ret;
	
	printk("completiontest_exit exit\n");
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

	if (!IS_ERR(tsk4))
	{
		ret = kthread_stop(tsk4);
		printk(KERN_INFO "thread function exit %d\n", ret);
	}
	
	return;
}

MODULE_AUTHOR("zlanda");
MODULE_LICENSE("GPL");

module_init(completiontest_init);
module_exit(completiontest_exit);

