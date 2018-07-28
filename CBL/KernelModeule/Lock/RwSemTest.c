/*******************************************************************************
*   文  件 名：RwSemTest.c
*   功     能：测试内核读写锁功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-28
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核读写锁机制说明
	读写信号量对访问者进行了细分，或者为读者，或者为写者，读者在保持读写信号量期间
只能对读写信号保护的共享资源进行访问，如果一个任务除了需要读，可能还需要写，它在对
共享资源访问之前必须先获得写者身份，写者在发现自己不需要写访问的情况下可以降级为读
者。读写信号量的访问规则：
	1、读写信号量同时拥有的读者数不受限制，也就是说可以有任意多个读者同时拥有一个
		读写信号量。
	2、如果一个读写信号量当前没有被写者拥有并且也没有写者等待读者释放信号量，那么
		任何读者	都可以成功获得该读写信号量；否则读者必须被挂起直到写者释放该信号量。
	3、如果一个读写信号量当前没有被读者或写者拥有并且也没有写者等待该信号量，那么
		一个写者可以成功获得该读写信号量，否则写者将被挂起，直到没有任何访问者。
		因此，写者是排他性的，独占性的。
		读写信号量有两种实现，一种是通用的，不依赖于硬件架构，因此，增加新的架构
		不需要重新实现它，但缺点是性能低，获得和释放读写信号量的开销大；另一种是架构
		相关的，因此性能高，获取和释放读写信号量的开销小，但增加新的架构需要重新
		实现。在内核配置时，可以通过选项去控制使用哪一种实现。

	读写信号量API：
	1、DECLARE_RWSEM(name)
		该宏声明一个读写信号量name并对其进行初始化
	2、void init_rwsem(struct rw_semaphore *sem);
		该函数对读写信号量sem进行初始化
	3、void down_read(struct rw_semaphore *sem);
		读者调用该函数来得到读写信号量sem。该函数会导致调用者睡眠，因此只能在进程
		上下文使用
	4、int down_read_trylock(struct rw_semaphore *sem);
		该函数类似于down_read，只是它不会导致调用者睡眠。它尽力得到读写信号量sem，
		如果能够立即得到，它就得到该读写信号量，并且返回1，否则表示不能立刻得到该
		信号量，返回0。因此，它也可以在中断上下文使用
	5、void down_write(struct rw_semaphore *sem);
		写者使用该函数来得到读写信号量sem，它也会导致调用者睡眠，因此只能在进程
		上下文使用
	6、int down_write_trylock(struct rw_semaphore *sem);
		该函数类似于down_write，只是它不会导致调用者睡眠。该函数尽力得到读写信号量，
		如果能够立刻获得，就获得该读写信号量并且返回1，否则表示无法立刻获得，返回0.
		它可以在中断上下文使用
	7、void up_read(struct rw_semaphore *sem);
		读者使用该函数释放读写信号量sem。它与down_read_trylock配对使用。如果
		down_read_trylock返回0，不需要调用up_read来释放读写信号量，因为根本就没有
		获得信号量。
	8、void up_write(struct rw_semaphore *sem);
		写者调用该函数释放信号量sem。它与down_write或down_write_trylock配对使用。
		如果down_write_trylock返回0，不需要调用up_write，因为返回0表示没有获得该
		读写信号量。
	9、void downgrade_write(struct rw_semaphore *sem);
		该函数用于把写者降级为读者，这有时是必要的。因为写者是排他性的，因此在写者
		保持读写信号量期间，任何读者或写者都将无法访问该读写信号量保护的共享资源，对于
		那些当前条件下不需要写访问的写者，降级为读者将使得等待访问的读者能够立刻
		访问，从而增加了并发性，提高了效率。

	读写信号量适于在读多写少的情况下使用，在Linux内核中对进程的内存映像描述结构的
访问就使用了读写信号量进行保护。在Linux中，每一个进程都用一个类型为task_t或
struct task_struct的结构来描述，该结构的类型中struct mm_struct的字段mm描述了进程
的内存映像，特别是mm_struct结构的mmap字段维护了整个进程的内存块保护，mmap_sem就是
一个读写信号量，在proc文件系统里有很多进程内存使用情况的接口，通过它们能够查看某
一进程的内存使用情况，命令free、ps和top都是通过proc来得到内存使用信息的，proc接口
就使用down_read和up_read来读取进程的mmap信息。当进程动态地分配或释放内存时，需要
修改mmap来反映分配或释放后的内存映像，因此动态内存分配或释放操作需要以写者身份获得
身份获得读写信号量mmap_sem来对mmap进行更新。系统调用brk和munmap就使用了down_write
和up_write来保护对mmap的访问。

*******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

struct rw_semaphore rwsem;

static int iCntNum = 0;

static struct task_struct *tsk1;		//读者
static struct task_struct *tsk2;		//读者
static struct task_struct *tsk3;		//写者

int thread_one(void *p);
int thread_two(void *p);
int thread_third(void *p);

int thread_one(void *p)
{
	static int iExeCnt = 0;
	
	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

        if (iExeCnt > 30)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			//读者用来获取sem，若没获得时，则调用者睡眠等待
			down_read(&rwsem);
			printk("thread_one: %d iExeCnt: %d\n", iCntNum, iExeCnt);
			//读者释放sem
			up_read(&rwsem);

			//休眠1s
			schedule_timeout(HZ);

			iExeCnt++;
		}
	}

	return 0;
}

int thread_two(void *p)
{
	static int iExeCnt = 0;
	
	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

        if (iExeCnt > 30)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			//读者用来获取sem，若没获得时，则调用者睡眠等待
			down_read(&rwsem);
			printk("thread_two: %d iExeCnt: %d\n", iCntNum, iExeCnt);
			//读者释放sem
			up_read(&rwsem);

			//休眠1s
			schedule_timeout(HZ);

			iExeCnt++;
		}
	}

	return 0;
}

int thread_third(void *p)
{
	static int iExeCnt = 0;

	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

		if (iExeCnt > 28)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			//写者用来获取sem，若没获得时，则调用者睡眠等待。
			down_write(&rwsem);
			iCntNum++;
			//写者释放sem
			up_write(&rwsem);

			//休眠1s
			schedule_timeout(HZ);

			iExeCnt++;
		}
	}

	return 0;
}

static int rwsemtest_init(void)
{
	printk("rwsemtest_init is coming\n");
	
	/* 对读写信号量sem进行初始化。 */
	init_rwsem(&rwsem);

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

static void rwsemtest_exit(void)
{
	int ret;
	
	printk("\rwsemtest_exit exit\n");
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

module_init(rwsemtest_init);
module_exit(rwsemtest_exit);

