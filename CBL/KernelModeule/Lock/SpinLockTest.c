/*******************************************************************************
*   文  件 名：SpinLockTest.c
*   功     能：测试内核自旋锁功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-28
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核自旋锁机制说明
	自旋锁与互斥锁有点类似，只是自旋锁不会引起调用者睡眠，如果自旋锁已经被别的执行
单元保持，调用者就一直循环在那里看是否该自旋锁的保持者已经释放了，“自旋”一词就是因此
而得名。由于自旋锁使用者一般保持锁时间非常短，因此选择自旋而不是睡眠是非常必要的，
自旋锁的效率远高于互斥锁。
	信号量和读写信号量适合于保持时间较长的情况，它们会导致调用者睡眠，因此只能在进程
上下文使用（_trylock的变种能够在中断上下文使用），而自旋锁适合于保持时间非常短的情况，
它可以在任何上下文使用。如果被保护的共享资源只在进程上下文访问，使用信号量保护该共享
资源非常合适，如果对共巷资源的访问时间非常短，自旋锁也可以。但是如果被保护的共享资源
需要在中断上下文访问（包括底半部即中断处理句柄和顶半部即软中断），就必须使用自旋锁。
	自旋锁保持期间是抢占失效的，而信号量和读写信号量保持期间是可以被抢占的。自旋锁只
有在内核可抢占或SMP的情况下才真正需要，在单CPU且不可抢占的内核下，自旋锁的所有操作都
是空操作，在单CPU且可抢占的内核下，自旋锁实际上只进行开启和关闭内核抢占的操作。
	跟互斥锁一样，一个执行单元要想访问被自旋锁保护的共享资源，必须先得到锁，在访问完
共享资源后，必须释放锁。如果在获取自旋锁时，没有任何执行单元保持该锁，那么将立即得到
锁；如果在获取自旋锁时锁已经有保持者，那么获取锁操作将自旋在那里，直到该自旋锁的保持
者释放了锁。
	无论是互斥锁，还是自旋锁，在任何时刻，最多只能有一个保持者，也就说，在任何时刻最
多只能有一个执行单元获得锁。

	自旋锁的API：
	1、spin_lock_init(x)
		该宏用于初始化自旋锁x。自旋锁在真正使用前必须先初始化。该宏用于动态初始化。
	2、DEFINE_SPINLOCK(x)
		该宏声明一个自旋锁x并初始化它。该宏在2.6.11中第一次被定义，在先前的内核中
		并没有该宏。
	3、SPIN_LOCK_UNLOCKED
		该宏用于静态初始化一个自旋锁。
		DEFINE_SPINLOCK(x)等同于spinlock_t x = SPIN_LOCK_UNLOCKED
	4、spin_is_locked(x)
		该宏用于判断自旋锁x是否已经被某执行单元保持（即被锁），如果是，返回真，
		否则返回假。
	5、spin_unlock_wait(x)
		该宏用于等待自旋锁x变得没有被任何执行单元保持，如果没有任何执行单元保持该自
		旋锁，该宏立即返回，否则将循环在那里，直到该自旋锁被保持者释放。
	6、spin_trylock(lock)
		该宏尽力获得自旋锁lock，如果能立即获得锁，它获得锁并返回真，否则不能立即获得
		锁，立即返回假。它不会自旋等待lock被释放。
	7、spin_lock(lock)
		该宏用于获得自旋锁lock，如果能够立即获得锁，它就马上返回，否则，它将自旋在那
		里，直到该自旋锁的保持者释放，这时，它获得锁并返回。总之，只有它获得锁才返回。
	8、spin_lock_irqsave(lock, flags)
		该宏获得自旋锁的同时把标志寄存器的值保存到变量flags中并失效本地中断。
	9、spin_lock_irq(lock)
		该宏类似于spin_lock_irqsave，只是该宏不保存标志寄存器的值。
	10、spin_lock_bh(lock)
		该宏在得到自旋锁的同时失效本地软中断。
	11、spin_unlock(lock)
		该宏释放自旋锁lock，它与spin_trylock或spin_lock配对使用。如果spin_trylock返回假，
		表明没有获得自旋锁，因此不必使用spin_unlock释放。
	12、spin_unlock_irqrestore(lock, flags)
		该宏释放自旋锁lock的同时，也恢复标志寄存器的值为变量flags保存的值。
		它与spin_lock_irqsave配对使用。
	13、spin_unlock_irq(lock)
		该宏释放自旋锁lock的同时，也使能本地中断。它与spin_lock_irq配对应用。
	14、spin_unlock_bh(lock)
		该宏释放自旋锁lock的同时，也使能本地的软中断。它与spin_lock_bh配对使用。
	15、spin_trylock_irqsave(lock, flags)
		该宏如果获得自旋锁lock，它也将保存标志寄存器的值到变量flags中，并且失效本地中
		断，如果没有获得锁，它什么也不做。因此如果能够立即获得锁，它等同于
		spin_lock_irqsave，如果不能获得锁，它等同于spin_trylock。如果该宏获得自旋锁
		lock，那需要使用spin_unlock_irqrestore来释放。
	16、spin_trylock_irq(lock)
		该宏类似于spin_trylock_irqsave，只是该宏不保存标志寄存器。如果该宏获得自旋锁
		lock，需要使用spin_unlock_irq来释放。
	17、spin_trylock_bh(lock)
		该宏如果获得了自旋锁，它也将失效本地软中断。如果得不到锁，它什么也不做。因此，
		如果得到了锁，它等同于spin_lock_bh，如果得不到锁，它等同于spin_trylock。
		如果该宏得到了自旋锁，需要使用spin_unlock_bh来释放。
	18、spin_can_lock(lock)
		该宏用于判断自旋锁lock是否能够被锁，它实际是spin_is_locked取反。如果lock没有
		被锁，它返回真，否则，返回假。该宏在2.6.11中第一次被定义，在先前的内核中并没
		有该宏。

	获得自旋锁和释放自旋锁有好几个版本，因此知道在什么样的情况下使用什么版本的获得和
	释放锁的宏是非常必要的。

	1、被保护的共享资源只在进程上下文访问和软中断上下文访问
		当在进程上下文访问共享资源时，可能被软中断打断，从而可能进入软中断上下文来对
	被保护的共享资源访问，因此对于这种情况，对共享资源的访问必须使用spin_lock_bh和
	spin_unlock_bh来保护。当然使用spin_lock_irq和spin_unlock_irq以及spin_lock_irqsave
	和spin_unlock_irqrestore也可以，它们失效了本地硬中断，失效硬中断隐式地也失效了软中断。
	但是使用spin_lock_bh和spin_unlock_bh是最恰当的，它比其他两个快。 
		举例说明：spinlock用在进程上下文和中断 
		进程A中调用了spin_lock（&lock）然后进入临界区，此时来了一个中断(interrupt），该
	中断也运行在和进程A相同的CPU上，并且在该中断处理程序中恰巧也会spin_lock(&lock)试图获取
	同一个锁。由于是在同一个CPU上被中断，进程A会被设置为TASK_INTERRUPT状态，中断处理程序无
	法获得锁，会不停的忙等，由于进程A被设置为中断状态，schedule（）进程调度就无法再调度进程
	A运行，这样就导致了死锁！但是如果该中断处理程序运行在不同的CPU上就不会触发死锁。因为在
	不同的CPU上出现中断不会导致进程A的状态被设为TASK_INTERRUPT，只是换出。当中断处理程序忙
	等被换出后，进程A还是有机会获得CPU，执行并退出临界区，(有一个问题没有搞懂关中断是关闭对
	所有cpu的中断还是本地cpu的？从关闭中断的函数来看似乎是针对本地的)，所以在使用spin_lock
	时要明确知道该锁不会在中断处理程序中使用。如果有，那就需要使用spinlock_irq_save，该函数
	即会关抢占，也会关本地中断(因为不能保证打断A的中断和A不是一个cpu，因此spin_lock在多核
	cpu上使用还是要关中断)。
	2、被保护的共享资源只在进程上下文和tasklet或timer上下文访问 
		应该使用与上面情况相同的获得和释放锁的宏，因为tasklet和timer是用软中断实现的。
	3、被保护的共享资源只在一个tasklet或timer上下文访问
		不需要任何自旋锁保护，因为同一个tasklet或timer只能在一个CPU上运行，即使是在SMP环境
	下也是如此。实际上tasklet在调用tasklet_schedule标记其需要被调度时已经把该tasklet绑定到
	当前CPU，因此同一个tasklet决不可能同时在其他CPU上运行。timer也是在其被使用add_timer添加
	到timer队列中时已经被帮定到当前CPU，所以同一个timer绝不可能运行在其他CPU上。当然同一个
	tasklet有两个实例同时运行在同一个CPU就更不可能了。
	4、被保护的共享资源只在两个或多个tasklet或timer上下文访问 
		对共享资源的访问仅需要用spin_lock和spin_unlock来保护，不必使用_bh版本，因为当
	tasklet或timer运行时，不可能有其他tasklet或timer在当前CPU上运行。 如果被保护的共享资源
	只在一个软中断（tasklet和timer除外）上下文访问，那么这个共享资源需要用spin_lock和
	spin_unlock来保护，因为同样的软中断可以同时在不同的CPU上运行。
	5、被保护的共享资源在两个或多个软中断上下文访问
		这个共享资源当然更需要用spin_lock和spin_unlock来保护，不同的软中断能够同时在不同
	的CPU上运行。
	6、被保护的共享资源在软中断（包括tasklet和timer）或进程上下文和硬中断上下文访问
		在软中断或进程上下文访问期间，可能被硬中断打断，从而进入硬中断上下文对共享资源进行
	访问，因此，在进程或软中断上下文需要使用spin_lock_irq和spin_unlock_irq来保护对共享资源
	的访问。而在中断处理句柄中使用什么版本，需依情况而定，如果只有一个中断处理句柄访问该共
	享资源，那么在中断处理句柄中仅需要spin_lock和spin_unlock来保护对共享资源的访问就可以了。
	因为在执行中断处理句柄期间，不可能被同一CPU上的软中断或进程打断。但是如果有不同的中断
	处理句柄访问该共享资源，那么需要在中断处理句柄中使用spin_lock_irq和spin_unlock_irq来
	保护对共享资源的访问。 
		在使用spin_lock_irq和spin_unlock_irq的情况下，完全可以用spin_lock_irqsave和
	spin_unlock_irqrestore取代，那具体应该使用哪一个也需要依情况而定，如果可以确信在对共享
	资源访问前中断是使能的，那么使用spin_lock_irq更好一些，因为它比spin_lock_irqsave要快一
	些，但是如果你不能确定是否中断使能，那么使用spin_lock_irqsave和spin_unlock_irqrestore
	更好，因为它将恢复访问共享资源前的中断标志而不是直接使能中断。当然，有些情况下需要在访
	问共享资源时必须中断失效，而访问完后必须中断使能，这样的情形使用spin_lock_irq和
	spin_unlock_irq最好。 
		需要特别提醒读者，spin_lock用于阻止在不同CPU上的执行单元对共享资源的同时访问以及
	不同进程上下文互相抢占导致的对共享资源的非同步访问，而中断失效和软中断失效却是为了阻止
	在同一CPU上软中断或中断对共享资源的非同步访问。 
		对于spin_lock用于阻止不同CPU上的执行单元对共享资源的同时访问以及不同进程上下文互相
	抢占导致的对共享资源的非同步访问在单核和多核cpu上的实现是不同的：
	A、单核cpu
		如果spin_lock不处于中断上下文，则spin_lock锁定的代码只会在内核发生抢占2的时候才会
	丢失CPU拥有权。所以，对于单核来说，需要在spin_lock获得锁的时候禁止抢占，释放锁的时候
	开放抢占。因此这不是真正意义上的锁。
	B、多核
		存在临界区同时在多核上被执行的情况，这时候才需要一个真正的锁来宣告代码对资源的占有。
	几个核可能会同时access临界区，这时的spinlock是如何实现的呢？ 
		要用到CPU提供的一些特殊指令，对lock变量进行原子操作。

*******************************************************************************/

/*******************************************************************************
	1、自旋锁作用与基本使用方法？
		与其他锁一样，自旋锁也用于保护临界区，但是自旋锁主要是用于在SMP上保护临界区。
	在SMP上，自旋锁最多只能被一个可执行线程持有，如果一个线程尝试获得一个被争用的自旋锁，
	该线程将一直旋转(while循环）直到锁可用；如果锁未被争用，请求锁的执行线程将立刻争用它，
	并继续执行。
		LINUX下自旋锁的基本使用方法：
		声明锁：
			spinlock_t lock;
		初始化：
			lock = SPIN_LOCK_UNLOCKED;
			或者
			spin_lock_init(&lock);
		加锁有4个接口,3个会阻塞，1个不阻塞
			spin_lock(&lock);//获取自旋锁
			spin_lock_irq(&lock);//关中断，获取自旋锁,不建议使用
			spin_lock_irqsave(&lock, flags);//关中断，保存中断状态，获取自旋锁
			spin_trylock(&lock);//与spin_lock一样，但是获取不到的时候不阻塞，返回非0
		对应的解锁接口有3个
			spin_unlock(&lock);//spin_lock和spin_trylock都用该接口解锁
			spin_unlock_irq(&lock);//不建议使用
			spin_unlock_irqrestore(&lock, flags);
		另外还提供了一个获取锁状态的接口：
			spin_is_locked(&lock);//如果指定的锁被获取，返回非0，否则，返回0
	2、在SMP和UP上的不同表现?
		对于SMP，自旋锁将在禁止抢占后，while循环自旋直到锁可用;
		对于UP，自旋锁的行为有点不一样，具体表现为：
			1、内核不支持抢占，则spin_lock是个空函数,spin_unlock也是空函数；
			2、内核支持抢占，则spin_lock关抢占，spin_unlock开抢占。
		要分析SMP和UP的区别，还得先理解UP下内核支持抢占与否的区分。
		在不支持抢占的时候，内核调度时机为：
			1、内核代码一直要执行到完成（返回用户空间）；
			2、阻塞或主动调用schedule();
		支持内核抢占的内核，则在以下情况会发生抢占：
			1、从中断处理程序回到内核空间且内核具有抢占性时；
			2、当内核代码再一次具有可抢占性时；(如时钟中断，发现进程时间片已经用完，则发生进程抢占)
			3、内核显式调用schedule();
			4、内核中的任务阻塞（同样导致schedule())；
		从以上区别可以看出，若是内核不支持抢占，只要临界区的代码不阻塞，就能保证原子性，
		所以spin_lock/spin_unlock是空函数。
		而对于支持抢占的内核， spin_lock/spin_unlock自然只需要关闭/恢复抢占功能即可。

	3、自旋锁与上下文
		由于自旋锁不睡眠，既可以用于进程上下文，也可用于非进程上下文，这是它与信号量相比的
		一个优势。正常来讲，如果自旋锁都是在进程上下文中使用，则建议使用
		spin_lock/spin_unlock。
		若在中断处理例程中也要使用自旋锁，则所有争用同一个锁的地方应使用
		spin_lock_irqsave/spin_unlock_irqrestore。
		若没有在中断而在下半部使用自旋锁，则所有急用同一个锁的地方可以使用
		spin_lock_bh/spin_unlock_bh，这对函数平时用得比较少。
		注：自旋锁用在中断例程中，若是进程上下文中的内核代码使用spin_lock，则在临界区，
		可能发生中断 ，要么原子性被破坏(UP)，要么造成死锁(SMP).

	4、使用spin_lock()后为什么不能睡眠?
		在UP下，正如前面所说，原子性得不到保证。而在SMP下，则可能发生死锁：
		假设有3个进程A,B,C，2个CPU称CPU0,CPU1.
		CPU0上A进程获取自旋锁进入睡眠，调度了B进程，B进程将自旋； 
		CPU1上调度了C进程也将自旋。
		B等着A释放锁，A等着B释放CPU，自旋后的CPU不能发生调度，CPU0和CPU1将一直自旋下去，
		形成了死锁。

	5、强调：锁什么?
		使用锁的时候一定要对症下药，明确保护的是数据而不是代码，这是使用锁的正确思考方式。
	经常看别人代码的人应该会有所体会，针对代码加锁常常使代码难以理解，特别是复杂的程序，
	往往加锁没做好，引起竞争条件。
		不防在使用锁的时候先思考一下自己要保护什么，然后给对应的锁取跟数据相关的名称。
	比如"int foo;spinlock_t foo_lock;"表示foo由foo_lock加锁;
	
*******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>

static int foo;
static spinlock_t foo_lock;

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
			spin_lock(&foo_lock);
			foo++;
			printk("thread_one: %d iExeCnt: %d\n", foo, iExeCnt);
			//释放自旋锁
			spin_unlock(&foo_lock);

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
			spin_lock(&foo_lock);
			foo++;
			printk("thread_two: %d iExeCnt: %d\n", foo, iExeCnt);
			//释放自旋锁
			spin_unlock(&foo_lock);

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
			spin_lock(&foo_lock);
			foo++;
			printk("thread_third: %d iExeCnt: %d\n", foo, iExeCnt);
			//释放自旋锁
			spin_unlock(&foo_lock);

			//休眠1s
			schedule_timeout(HZ);

			iExeCnt++;
		}
	}

	return 0;
}

static int spintest_init(void)
{
	printk("spintest_init is coming\n");
	
	/* 初始化自旋锁 */
	spin_lock_init(&foo_lock);

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

static void spintest_exit(void)
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

module_init(spintest_init);
module_exit(spintest_exit);

