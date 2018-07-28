/*******************************************************************************
*   文  件 名：SeqLockTest.c
*   功     能：测试内核顺序锁功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-28
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核顺序锁机制说明
	顺序锁也是对读写锁的一种优化，对于顺序锁，读者绝不会被写者阻塞，也就说，读者可以
在写者对被顺序锁保护的共享资源进行写操作时仍然可以继续读，而不必等待写者完成写操作，
写者也不需要等待所有读者完成读操作才去进行写操作。但是，写者与写者之间仍然是互斥的，
即如果有写者在进行写操作，其他写者必须自旋在那里，直到写者释放了顺序锁。 
	这种锁有一个限制，它必须要求被保护的共享资源不含有指针，因为写者可能使得指针失效，
但读者如果正要访问该指针，将导致OOPs。 
	如果读者在读操作期间，写者已经发生了写操作，那么，读者必须重新读取数据，以便确保
得到的数据是完整的。 
	这种锁对于读写同时进行的概率比较小的情况，性能是非常好的，而且它允许读写同时进行，
因而更大地提高了并发性。

	顺序锁的API：
	1、void write_seqlock(seqlock_t *sl);
		写者在访问被顺序锁s1保护的共享资源前需要调用该函数来获得顺序锁s1。它实际功能
	上等同于spin_lock，只是增加了一个对顺序锁顺序号的加1操作，以便读者能够检查出是否
	在读期间有写者访问过。
	2、void write_sequnlock(seqlock_t *sl);
		写者在访问完被顺序锁s1保护的共享资源后需要调用该函数来释放顺序锁s1。它实际功
	能上等同于spin_unlock，只是增加了一个对顺序锁顺序号的加1操作，以便读者能够检查出
	是否在读期间有写者访问过。 
		写者使用顺序锁的模式如下：
			write_seqlock(&seqlock_a);
			//写操作代码块
			…
			write_sequnlock(&seqlock_a);
		因此，对写者而言，它的使用与spinlock相同。
	3、int write_tryseqlock(seqlock_t *sl);
		写者在访问被顺序锁s1保护的共享资源前也可以调用该函数来获得顺序锁s1。它实际功
	能上等同于spin_trylock，只是如果成功获得锁后，该函数增加了一个对顺序锁顺序号的加
	1操作，以便读者能够检查出是否在读期间有写者访问过。
	4、unsigned read_seqbegin(const seqlock_t *sl);
		读者在对被顺序锁s1保护的共享资源进行访问前需要调用该函数。读者实际没有任何得
	到锁和释放锁的开销，该函数只是返回顺序锁s1的当前顺序号。
	5、int read_seqretry(const seqlock_t *sl, unsigned iv);
		读者在访问完被顺序锁s1保护的共享资源后需要调用该函数来检查，在读访问期间是否
	有写者访问了该共享资源，如果是，读者就需要重新进行读操作，否则，读者成功完成了读操作。 
		因此，读者使用顺序锁的模式如下：
			do {
			seqnum = read_seqbegin(&seqlock_a);
			//读操作代码块
			...
			} while (read_seqretry(&seqlock_a, seqnum));
	6、write_seqlock_irqsave(lock, flags)
		写者也可以用该宏来获得顺序锁lock，与write_seqlock不同的是，该宏同时还把标志寄存器
	的值保存到变量flags中，并且失效了本地中断。
	7、write_seqlock_irq(lock)
		写者也可以用该宏来获得顺序锁lock，与write_seqlock不同的是，该宏同时还失效了本地
	中断。与write_seqlock_irqsave不同的是，该宏不保存标志寄存器。
	8、write_seqlock_bh(lock)
		写者也可以用该宏来获得顺序锁lock，与write_seqlock不同的是，该宏同时还失效了本地
	软中断。
	9、write_sequnlock_irqrestore(lock, flags)
		写者也可以用该宏来释放顺序锁lock，与write_sequnlock不同的是，该宏同时还把标志寄
	存器的值恢复为变量flags的值。它必须与write_seqlock_irqsave配对使用。
	10、write_sequnlock_irq(lock)
		写者也可以用该宏来释放顺序锁lock，与write_sequnlock不同的是，该宏同时还使能本地
	中断。它必须与write_seqlock_irq配对使用。
	11、write_sequnlock_bh(lock)
		写者也可以用该宏来释放顺序锁lock，与write_sequnlock不同的是，该宏同时还使能本地
	软中断。它必须与write_seqlock_bh配对使用。
	12、read_seqbegin_irqsave(lock, flags)
		读者在对被顺序锁lock保护的共享资源进行访问前也可以使用该宏来获得顺序锁lock的当前
	顺序号，与read_seqbegin不同的是，它同时还把标志寄存器的值保存到变量flags中，并且失效
	了本地中断。注意，它必须与read_seqretry_irqrestore配对使用。
	13、read_seqretry_irqrestore(lock, iv, flags)
		读者在访问完被顺序锁lock保护的共享资源进行访问后也可以使用该宏来检查，在读访问期
	间是否有写者访问了该共享资源，如果是，读者就需要重新进行读操作，否则，读者成功完成了
	读操作。它与read_seqretry不同的是，该宏同时还把标志寄存器的值恢复为变量flags的值。
	注意，它必须与read_seqbegin_irqsave配对使用。 
		因此，读者使用顺序锁的模式也可以为：
			do {
			   seqnum = read_seqbegin_irqsave(&seqlock_a, flags);
			//读操作代码块
			...
			} while (read_seqretry_irqrestore(&seqlock_a, seqnum, flags));
		读者和写者所使用的API的几个版本应该如何使用与自旋锁的类似。 
		如果写者在操作被顺序锁保护的共享资源时已经保持了互斥锁保护对共享数据的写操作，即
	写者与写者之间已经是互斥的，但读者仍然可以与写者同时访问，那么这种情况仅需要使用顺序
	计数（seqcount），而不必要spinlock。



	顺序计数的API:
	1、unsigned read_seqcount_begin(const seqcount_t *s);
		读者在对被顺序计数保护的共享资源进行读访问前需要使用该函数来获得当前的顺序号。
	2、int read_seqcount_retry(const seqcount_t *s, unsigned iv);
		读者在访问完被顺序计数s保护的共享资源后需要调用该函数来检查，在读访问期间是否
	有写者访问了该共享资源，如果是，读者就需要重新进行读操作，否则，读者成功完成了读操作。 
		因此，读者使用顺序计数的模式如下：
			do {
			   seqnum = read_seqbegin_count(&seqcount_a);
			//读操作代码块
			...
			} while (read_seqretry(&seqcount_a, seqnum));
	3、void write_seqcount_begin(seqcount_t *s);
		写者在访问被顺序计数保护的共享资源前需要调用该函数来对顺序计数的顺序号加1，以便
	读者能够检查出是否在读期间有写者访问过。
	4、void write_seqcount_end(seqcount_t *s);
		写者在访问完被顺序计数保护的共享资源后需要调用该函数来对顺序计数的顺序号加1，以
	便读者能够检查出是否在读期间有写者访问过。 
		写者使用顺序计数的模式为：
			write_seqcount_begin(&seqcount_a);
			//写操作代码块
			…
			write_seqcount_end(&seqcount_a);
		需要特别提醒，顺序计数的使用必须非常谨慎，只有确定在访问共享数据时已经保持了互斥
	锁才可以使用。 
		内核在写和读netdevice的名字时分别使用dev_change_name和netdev_get_name中使用了该
	方法。而dev_change_name由dev_ifsioc调用，其如下所示使用互斥锁进行了保护。
		rtnl_lock(); //内部即互斥锁
		ret = dev_ifsioc(net, &ifr, cmd)
		rtnl_unlock();
		#define local_irq_enable()  do { raw_local_irq_enable(); } while (0)
		#define local_irq_disable() do { raw_local_irq_disable(); } while (0)
		#define raw_local_irq_disable()     arch_local_irq_disable()
		#define raw_local_irq_enable()      arch_local_irq_enable()
		可以看到这个与体系结构有关

		禁止内核抢占就比较简单了，就是防止当前进程不会突然被另一个进程抢占。在Linux的实现
	就是preempt_disable()和preempt_enable()函数
			#define preempt_disable() \  
			do { \  
			    //增加preempt_count  
			    inc_preempt_count(); \  
			    //保证先加了preempt_count才进行以后的操作  
			    barrier(); \  
			} while (0)  

			#define preempt_enable() \  
			do { \  
			    preempt_enable_no_resched(); \  
			    barrier(); \  
			    //检查当前进程是否可抢占  
			    preempt_check_resched(); \  
			} while (0) 
			
		不管是禁止中断还是禁止内核抢占，都是为了提供内核同步，但是他们都没有提供任何保护
	机制来防止其它处理器的并发访问。Linux支持多处理器，因此，内核代码一般都需要获取某种锁，
	防止来自其他处理器对共享数据的并发访问，而禁止中断提供保护机制，这是防止来自其他中断
	处理程序的并发访问。
		前面说的都是概念，现在我们来讨论几个问题 
		1.在单处理器条件下，为什么禁止中断就可以禁止内核抢占？ 
			首先来回顾一下内核抢占发生在哪些时候： 
			1. 在中断返回内核空间的时候，这个没什么好说的，跟中断密切相关，没了中断就不会
		发生 
			2. 内核显式调用schedule()（可抢占或阻塞） 
		我们先搞清楚一件事，就是我们说禁止中断可以禁止内核抢占只是说禁止任何意外的抢占，
		如果进程自己要调用schedule函数，那谁也拦不住，事实上调用schedule这个函数本来就要
		禁止中断，所以剩下的就是考虑创建或者唤醒一个更高优先级的进程，或者调用信号量、
		完成量，所有的这些情况都要通过try_to_wake_up函数唤醒另一个进程，但是这个函数真正
		干的事只是设置了一下need_resched这个函数，并没有真的调用schedule函数，调用是在系统
		调用返回用户空间的时候进行的，所以跟内核抢占也没啥关系，所以从这些方面来说，禁止
		中断是可以禁止内核抢占的 
		2.自旋锁关中断后，为什么要再禁止抢占? 
		假设有这么个情况： 
			1、CPU-1在进程A的上下文调用了spin_lock_irqsave； 
			2、CPU-2调用wake_up_process唤醒了CPU-1上的进程B，由于进程B的优先级高于进程A，
				进程A的TIF_NEED_RESCHED标记被设置。（CPU-2还会用IPI通知CPU-1进行resched，
				但是CPU-1禁用了中断而不会响应）； 
			3、CPU-1调用了某某函数，这个函数包含了preempt_disable和preempt_enable（没有规
				定关中断的情况下不能调用这样的函数吧~）； 
		那么，如果spin_lock_irqsave没有preempt_disable，第3步中的preempt_enable将触发
		preempt_check_resched，从而让进程B抢占掉进程A。 
		总之就是只有关了抢占，才能保证在临界区成对出现的preempt_disable()/preempt_enable()
		（preempt_enable()也是一个潜在的主动调度的测试点）不会造成伤害。不然这种代码就不能
		放在临界区中了。

*******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/seqlock.h>

static int foo;
static seqlock_t foo_lock;

static struct task_struct *tsk1;		//读者
static struct task_struct *tsk2;		//读者
static struct task_struct *tsk3;		//写者

int thread_one(void *p);
int thread_two(void *p);
int thread_third(void *p);

int thread_one(void *p)
{
	static int iExeCnt = 0;
	int seqnum = 0;
	
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
			do 
			{
				seqnum = read_seqbegin(&foo_lock);
				printk("thread_one: %d seqnum: %d\n", foo, seqnum);
			} while (read_seqretry(&foo_lock, seqnum));

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
	int seqnum = 0;
	
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
			//标准套路
			do 
			{
				seqnum = read_seqbegin(&foo_lock);
				printk("thread_two: %d seqnum: %d\n", foo, seqnum);
			} while (read_seqretry(&foo_lock, seqnum));

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

		if (iExeCnt > 30)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			//获取自旋锁
			write_seqlock(&foo_lock);
			foo++;
			printk("thread_third: %d iExeCnt: %d\n", foo, iExeCnt);
			//释放自旋锁
			write_sequnlock(&foo_lock);

			//休眠1s
			schedule_timeout(HZ);

			iExeCnt++;
		}
	}

	return 0;
}

static int seqtest_init(void)
{
	printk("seqtest_init is coming\n");
	
	/* 初始化自旋锁 */
	seqlock_init(&foo_lock);

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

static void seqtest_exit(void)
{
	int ret;
	
	printk("seqtest_exit exit\n");
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

module_init(seqtest_init);
module_exit(seqtest_exit);

