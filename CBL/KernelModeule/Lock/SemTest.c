/*******************************************************************************
*   文  件 名：SemTest.c
*   功     能：测试内核信号量功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-27
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核信号量机制说明
	Linux内核的信号量在概念和原理上与用户态的System V的IPC机制信号量是一样的，但是
它绝不可能在内核之外使用，因此它与Ststem V的IPC机制信号量毫不相干。
	信号量在创建时需要设置一个初始值，表示同时可以有几个任务可以访问该信号量保护的
共享资源，初始值为1就变成互斥锁（Mutex），即同时只能有一个任务可以访问信号量保护的
共享资源。一个任务要想访问共享资源，首先必须得到信号量，获取信号量的操作将把信号量
的值减1，若当前信号量的值为负数，表面无法获得信号量，该任务必须挂起在该信号量的等待
队列等待该信号量可用；若当前信号量的值为非负数，表示可以获得信号量，因而可以立刻
访问被改信号量保护的共享资源。当任务访问完被信号量保护的共享资源后，必须释放信号量，
释放信号量通过把信号量的值加1实现，如果信号量的值为非正数，表明有任务等待当前信号量，
因此它也唤醒所有等待该信号量的任务。
	信号量API：
	1、DEFINE_MUTEX
		静态定义和初始化一个互斥锁
	2、void mutex_init(struct mutex *mutex);
		动态初始化一个互斥锁
	3、void sema_init (struct semaphore *sem, int val);
		该函数用于初始化设置信号量的初值，它设置信号量sem的值为val。
		尽管信号量可以被初始化为大于1的值从而成为一个计数信号量，但是它通常不被
	这样使用。内核定义了两个宏来把sem的值设置为1或者0.
		//在最新的内核上这两个宏已经被废弃，但是实际意义是一样的。
		#define init_MUTEX(sem)                  sema_init(sem, 1)
		#define init_MUTEX_LOCKED(sem)         sema_init(sem, 0)
		使用init_MUTEX(sem)初始化信号量时，表示信号量最初是可以被获取的；而是要
		init_MUTEX_LOCKED(sem)初始化信号量时，此信号量只有先辈释放才可以获取。
		释放信号量将该值加1
	4、void down(struct semaphore * sem);
		该函数用于获得信号量sem，它会导致睡眠，因此不能再中断上下文（包括IRQ上下文
	和softirq上下文）使用该函数。该函数首先判断sem->count的值是否大于0，如果ture
	则sem->count -1，否则调用者将被挂起，直到别的任务释放该信号量才能继续运行。
	5、int down_interruptible(struct semaphore * sem);
		该函数功能与down类似，不同之处为，down不会被信号(signal)打断，但down_interruptible
	能被信号打断，因此该函数有返回值来区分是正确返回还是被信号量中断，如果返回0，
	表示获得信号量正常返回，如果被信号打断，返回-EINTR。
		这个函数的功能就是获得信号量，如果得不到信号量就睡眠，此时没有信号打断，
	那么进入睡眠。但是在睡眠过程中可能被信号打断，打断之后返回-EINTR，主要用来进程间
	的互斥同步。
		一个进程在调用down_interruptible()之后，如果sem<0,那么就进入到可中断的睡眠状
	态并调度其它进程运行， 但是一旦该进程收到信号，那么就会从down_interruptible函数中
	返回。并标记错误号为:-EINTR。一个形象的比喻：传入的信号量为1好比天亮，如果当前信
	号量为0，进程睡眠，直到（信号量为1）天亮才醒，但是可能中途有个闹铃（信号）把你闹
	醒。又如：小强下午放学回家，回家了就要开始吃饭嘛，这时就会有两种情况：情况一：饭
	做好了，可以开始吃；情况二：当他到厨房去的时候发现妈妈还在做，妈妈就对他说：“你先
	去睡会，待会做好了叫你。”小强就答应去睡会，不过又说了一句：“睡的这段时间要是小红
	来找我玩，你可以叫醒我。”小强就是down_interruptible，想吃饭就是获取信号量，睡觉
	对应这里的休眠，而小红来找我玩就是中断休眠。
		使用可被中断的信号量版本的意思是，万一出现了semaphore的死锁，还有机会
	用ctrl+c发出软中断，让等待这个内核驱动返回的用户态进程退出。而不是把整个系统都锁
	住了。在休眠时，能被中断信号终止，这个进程是可以接受中断信号的！比如你在命令行中
	输入# sleep 10000，按下ctrl + c，就给上面的进程发送了进程终止信号。信号发送给用户
	空间，然后通过系统调用，会把这个信号传给递给驱动。信号只能发送给用户空间，无权直
	接发送给内核的，那1G的内核空间，我们是无法直接去操作的。
	6、int down_trylock(struct semaphore * sem);
		该函数试着获得信号sem，如果能够立刻获得，它就获得该信号量并返回0，否则，
	表示不能获得信号量sem，返回值为非0值。因此，它不会导致调用者睡眠，可以在中断
	上下文使用。
	7、void up(struct semaphore * sem);
		该函数释放信号量sem，即把sem的值加1，如果sem的值为非正数，表明有任务等待
	该信号量，因此唤醒这些等待者。信号量在绝大部分情况下作为互斥锁使用。
*******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

struct semaphore sem1;
struct semaphore sem2;
static struct task_struct *tsk1;
static struct task_struct *tsk2;


int num[2][5] = {
	{0,2,4,6,8},
	{1,3,5,7,9}
};

int thread_one(void *p);
int thread_two(void *p);
int thread_one(void *p)
{
	int *num = (int *)p;
	int i;
	static int iExeCnt = 0;
	
	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

        if (iExeCnt != 0)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			for(i = 0; i < 5; i++)
			{
				//因为初始化的原因，sem1初始化后即可被获取
				down(&sem1);				//获取信号量1
				printk("thread_one: %d \n", num[i]);
				up(&sem2);					//释放信号量2
			}

			iExeCnt++;
		}
	}

	return 0;
}

int thread_two(void *p)
{
	int *num = (int *)p;
	int i;
	static int iExeCnt = 0;

	while (1)
	{
		set_current_state(TASK_UNINTERRUPTIBLE);

		if (kthread_should_stop())
        {
			break;
        }

		if (iExeCnt != 0)
		{
			schedule_timeout(2 * HZ);
		}
		else
		{
			for(i = 0; i < 5; i++)
			{
				//因为初始化的原因，sem2只有在被释放后才能被获取，因此它要等到线程1将其释放后才能获取
				//被阻塞
				down(&sem2);				//获取信号量2
				printk("thread_two: %d \n", num[i]);
				up(&sem1);					//释放信号量1
			}

			iExeCnt++;
		}
	}

	return 0;
}

static int semtest_init(void)
{
	printk("semtest_init is coming\n");
	/* 注意：init_MUTEX(sem)和init_MUTEX_LOCKED(sem)在新版本内核中已经被废除 */
	/*
		解决办法：
		#define init_MUTEX(sem)     sema_init(sem, 1)
		#define init_MUTEX_LOCKED(sem)  sema_init(sem, 0)
	*/
	sema_init(&sem1, 1);  //初始化信号量1， 使信号量1最初可被获取
	sema_init(&sem2, 0);  //初始化信号量2，使信号量2只有被释放后才可被获取

	//先将tsk2运行起来，实际上因为信号量的原因，它要等到tsk1运行后才能被运行
	tsk2 = kthread_run(thread_two, num[1], "mythread%d", 3);
	if (IS_ERR(tsk2)) 
	{
		printk(KERN_INFO "create kthread tsk2 failed!\n");
	}
	else 
	{
		printk(KERN_INFO "create ktrhead tsk2 ok!\n");
	}
	
	tsk1 = kthread_run(thread_one, num[0], "mythread%d", 2);
	if (IS_ERR(tsk1)) 
	{
		printk(KERN_INFO "create kthread tsk1 failed!\n");
	}
	else 
	{
		printk(KERN_INFO "create ktrhead tsk1 ok!\n");
	}
	
	return 0;
}

static void semtest_exit(void)
{
	int ret;
	
	printk("\nsemtest_exit exit\n");
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

	return;
}

module_init(semtest_init);
module_exit(semtest_exit);

