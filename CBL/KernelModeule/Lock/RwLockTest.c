/*******************************************************************************
*   文  件 名：RwLockTest.c
*   功     能：测试内核读写锁功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-28
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核读写锁机制说明
	读写锁实际是一种特殊的自旋锁，它把对共享资源的访问者划分成读者和写者，读者只对共享
资源进行读访问，写者则需要对共享资源进行写操作。这种锁相对于自旋锁而言，能提高并发性，
因为在多处理器系统中，它允许同时有多个读者来访问共享资源，最大可能的读者数为实际的逻辑
CPU数。写者是排他性的，一个读写锁同时只能有一个写者或多个读者（与CPU数相关），但不能同
时既有读者又有写者。 
	在读写锁保持期间也是抢占失效的。 
	读写锁访问规则:
	1、如果读写锁当前没有读者，也没有写者，那么写者可以立刻获得读写锁，否则它必须自旋在
		那里，直到没有任何写者或读者。
	2、如果读写锁没有写者，那么读者可以立即获得该读写锁，否则读者必须自旋在那里，直到写
		者释放该读写锁。

	读写锁API：
	读写锁的API看上去与自旋锁很像，只是读者和写者需要不同的获得和释放锁的API。
	1、rwlock_init(x)
		该宏用于动态初始化读写锁x。
	2、DEFINE_RWLOCK(x)
		该宏声明一个读写锁并对其进行初始化。它用于静态初始化。
	3、RW_LOCK_UNLOCKED
		它用于静态初始化一个读写锁。 
		DEFINE_RWLOCK(x)等同于rwlock_t x = RW_LOCK_UNLOCKED
	4、read_trylock(lock)
		读者用它来尽力获得读写锁lock，如果能够立即获得读写锁，它就获得锁并返回真，
	否则不能获得锁，返回假。无论是否能够获得锁，它都将立即返回，绝不自旋在那里。
	5、write_trylock(lock)
		写者用它来尽力获得读写锁lock，如果能够立即获得读写锁，它就获得锁并返回真，
	否则不能获得锁，返回假。无论是否能够获得锁，它都将立即返回，绝不自旋在那里。
	6、read_lock(lock)
		读者要访问被读写锁lock保护的共享资源，需要使用该宏来得到读写锁lock。如果
	能够立即获得，它将立即获得读写锁并返回，否则，将自旋在那里，直到获得该读写锁。
	7、write_lock(lock)
		写者要想访问被读写锁lock保护的共享资源，需要使用该宏来得到读写锁lock。如果
	能够立即获得，它将立即获得读写锁并返回，否则，将自旋在那里，直到获得该读写锁。
	8、read_lock_irqsave(lock, flags)
		读者也可以使用该宏来获得读写锁，与read_lock不同的是，该宏还同时把标志寄存
	器的值保存到了变量flags中，并失效了本地中断。
	9、write_lock_irqsave(lock, flags)
		写者可以用它来获得读写锁，与write_lock不同的是，该宏还同时把标志寄存器的值
	保存到了变量flags中，并失效了本地中断。
	10、read_lock_irq(lock)
		读者也可以用它来获得读写锁，与read_lock不同的是，该宏还同时失效了本地中断。
	该宏与read_lock_irqsave的不同之处是，它没有保存标志寄存器。
	11、write_lock_irq(lock)
		写者也可以用它来获得锁，与write_lock不同的是，该宏还同时失效了本地中断。该
	宏与write_lock_irqsave的不同之处是，它没有保存标志寄存器。
	12、read_lock_bh(lock)
		读者也可以用它来获得读写锁，与与read_lock不同的是，该宏还同时失效了本地的
	软中断。
	13、write_lock_bh(lock)
		写者也可以用它来获得读写锁，与write_lock不同的是，该宏还同时失效了本地的
	软中断。
	14、read_unlock(lock)
		读者使用该宏来释放读写锁lock。它必须与read_lock配对使用。
	15、write_unlock(lock)
		写者使用该宏来释放读写锁lock。它必须与write_lock配对使用。
	16、read_unlock_irqrestore(lock, flags)
		读者也可以使用该宏来释放读写锁，与read_unlock不同的是，该宏还同时把标志
	寄存器的值恢复为变量flags的值。它必须与read_lock_irqsave配对使用。
	17、write_unlock_irqrestore(lock, flags)
		写者也可以使用该宏来释放读写锁，与write_unlock不同的是，该宏还同时把标志
	寄存器的值恢复为变量flags的值，并使能本地中断。它必须与write_lock_irqsave配对
	使用。
	18、read_unlock_irq(lock)
		读者也可以使用该宏来释放读写锁，与read_unlock不同的是，该宏还同时使能本地
	中断。它必须与read_lock_irq配对使用。
	19、write_unlock_irq(lock)
		写者也可以使用该宏来释放读写锁，与write_unlock不同的是，该宏还同时使能本地
	中断。它必须与write_lock_irq配对使用。
	20、read_unlock_bh(lock)
		读者也可以使用该宏来释放读写锁，与read_unlock不同的是，该宏还同时使能本地
	软中断。它必须与read_lock_bh配对使用。
	21、write_unlock_bh(lock)
		写者也可以使用该宏来释放读写锁，与write_unlock不同的是，该宏还同时使能本地
	软中断。它必须与write_lock_bh配对使用。 

	读写锁的获得和释放锁的方法也有许多版本，具体用哪个与自旋锁一样，因此参考自旋锁
	部分就可以了。只是需要区分读者与写者，读者要用读者版本，而写者必须用写者版本。
	
*******************************************************************************/

/*******************************************************************************
						大读者锁（brlock-Big Reader Lock）
	大读者锁是读写锁的高性能版，读者可以非常快地获得锁，但写者获得锁的开销比较大。
大读者锁只存在于2.4内核中，在2.6中已经没有这种锁（提醒读者特别注意）。它们的使用
与读写锁的使用类似，只是所有的大读者锁都是事先已经定义好的。这种锁适合于读多写少
的情况，它在这种情况下远好于读写锁。 
	大读者锁的实现机制是：每一个大读者锁在所有CPU上都有一个本地读者写者锁，一个读者
仅需要获得本地CPU的读者锁，而写者必须获得所有CPU上的锁。

	大读者锁API：
	大读者锁的API非常类似于读写锁，只是锁变量为预定义的锁ID。
	1、void br_read_lock (enum brlock_indices idx);
		读者使用该函数来获得大读者锁idx，在2.4内核中，预定义的idx允许的值有两个：
	BR_GLOBALIRQ_LOCK和BR_NETPROTO_LOCK，当然，用户可以添加自己定义的大读者锁ID 。
	2、void br_read_unlock (enum brlock_indices idx);
		读者使用该函数释放大读者锁idx。
	3、void br_write_lock (enum brlock_indices idx);
		写者使用它来获得大读者锁idx。
	4、void br_write_unlock (enum brlock_indices idx);
		写者使用它来释放大读者锁idx。
	5、br_read_lock_irqsave(idx, flags)
		读者也可以使用该宏来获得大读者锁idx，与br_read_lock不同的是，该宏还同时
	把寄存器的值保存到变量flags中，并且失效本地中断。
	6、br_read_lock_irq(idx)
		读者也可以使用该宏来获得大读者锁idx，与br_read_lock不同的是，该宏还同时
	失效本地中断。与br_read_lock_irqsave不同的是，该宏不保存标志寄存器。
	7、br_read_lock_bh(idx)
		读者也可以使用该宏来获得大读者锁idx，与br_read_lock不同的是，该宏还同时
	失效本地软中断。
	8、br_write_lock_irqsave(idx, flags)
		写者也可以使用该宏来获得大读者锁idx，与br_write_lock不同的是，该宏还同时
	把寄存器的值保存到变量flags中，并且失效本地中断。
	9、br_write_lock_irq(idx)
		写者也可以使用该宏来获得大读者锁idx，与br_write_lock不同的是，该宏还同时
	失效本地中断。与br_write_lock_irqsave不同的是，该宏不保存标志寄存器。
	10、br_write_lock_bh(idx)
		写者也可以使用该宏来获得大读者锁idx，与br_write_lock不同的是，该宏还同时
	失效本地软中断。
	11、br_read_unlock_irqrestore(idx, flags)
		读者也使用该宏来释放大读者锁idx，它与br_read_unlock不同之处是，该宏还同时
	把变量flags的值恢复到标志寄存器。
	12、br_read_unlock_irq(idx)
		读者也使用该宏来释放大读者锁idx，它与br_read_unlock不同之处是，该宏还同时
	使能本地中断。
	13、br_read_unlock_bh(idx)
		读者也使用该宏来释放大读者锁idx，它与br_read_unlock不同之处是，该宏还同时
	使能本地软中断。
	14、br_write_unlock_irqrestore(idx, flags)
		写者也使用该宏来释放大读者锁idx，它与br_write_unlock不同之处是，该宏还同时
	把变量flags的值恢复到标志寄存器。
	15、br_write_unlock_irq(idx)
		写者也使用该宏来释放大读者锁idx，它与br_write_unlock不同之处是，该宏还同时
	使能本地中断。
	16、br_write_unlock_bh(idx)
		写者也使用该宏来释放大读者锁idx，它与br_write_unlock不同之处是，该宏还同时
	使能本地软中断。 

	这些API的使用与读写锁完全一致。
*******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

static int foo;
static rwlock_t foo_lock;

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
			//获取自旋锁
			read_lock(&foo_lock);
			printk("thread_one: %d iExeCnt: %d\n", foo, iExeCnt);
			//释放自旋锁
			read_unlock(&foo_lock);

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
			//获取自旋锁
			read_lock(&foo_lock);
			printk("thread_two: %d iExeCnt: %d\n", foo, iExeCnt);
			//释放自旋锁
			read_unlock(&foo_lock);

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
			write_lock(&foo_lock);
			foo++;
			printk("thread_third: %d iExeCnt: %d\n", foo, iExeCnt);
			//释放自旋锁
			write_unlock(&foo_lock);

			//休眠1s
			schedule_timeout(HZ);

			iExeCnt++;
		}
	}

	return 0;
}

static int rwlocktest_init(void)
{
	printk("rwlocktest_init is coming\n");
	
	/* 初始化自旋锁 */
	rwlock_init(&foo_lock);

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

static void rwlocktest_exit(void)
{
	int ret;
	
	printk("rwlocktest_exit exit\n");
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

module_init(rwlocktest_init);
module_exit(rwlocktest_exit);

