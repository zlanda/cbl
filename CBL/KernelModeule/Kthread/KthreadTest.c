/*******************************************************************************
*   文  件 名：KthreadTest.c
*   功     能：实现内核线程功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-26
*   修改历史：无
*******************************************************************************/

//https://blog.csdn.net/gatieme/article/details/51589205

/*******************************************************************************
                                    说明
1、内核线程，指示一个称呼，实际上就是一个进程，有自己独立的TCB，参与内核调度，也
    参与内核抢占。这个进程的特别之处有两点。第一：该进程没有前台；第二：永远在内
    核态中运行。
2、创建内核线程有两种方法，一种是kthread_create()，一种是kernel_thread()
    内核线程可以用kernel_thread()创建，但是在执行函数里面必须用daemonize释放资源
    并挂到init下，还需要用completion等待这一过程的完成。
    kthread_create()是比较正牌的创建函数，这个不必要调用daemonize，用这个创建的
    内核线程都挂在了kthread线程下。
    从表面上来看，这两个函数非常的类似，但是实现却是相差甚远。
    kthread_create()是通过work_queue来实现的，kernel_thread()是通过do_fork来实现的。
    kthread_create:DECLARE_WORK(work, keventd_create_kthread, &create);
        调用keventd_create_kthread，keventd_create_kthread中调用kernel_thread：
        pid = kernel_thread(kthread, create, CLONE_FS | CLONE_FILES | SIGCHLD);
3、kernel_thread()的关键是调用在内核中实现的do_fork()，核心本质上还是一个进程。
4、kthread_run是kthread_create的宏。
5、kthread_create：创建线程。线程创建后，不会马上运行，而是需要将kthread_create() 
    返回的task_struct指针传给wake_up_process()，然后通过此函数运行线程。
   kthread_run ：创建并启动线程的函数
6、在模块卸载时，可以使用int kthread_stop(struct task_struct *k)结束线程的运行。
    注意：在调用kthread_stop函数时，线程函数不能已经运行结束。否则，kthread_stop
            函数会一直进行等待；
          线程函数必须能让出CPU，以便能运行其他线程。同时线程函数也必须能重新被
            调度运行；
7、可以通过ps afx命令显示所有内核线程，线程名由kthread_create函数的第三个参数指定。
   可以通过ps aux|grep 线程名 命令来查找线程号
   可以通过top -p 线程号 命令来查看线程（包括内核线程）的CPU利用率。

*******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>      //kthread_create()、kthread_run()
#include <linux/delay.h>
#include <linux/sched.h>        //wake_up_process()
#include <linux/err.h>

#define KTHREAD_RUN_TEST							0
#define KTHREAD_CREATE_TEST							1

#if KTHREAD_RUN_TEST

static struct task_struct *tsk;
static int param_data;

static int thread_function(void *data)
{
    int time_count = 0;
    int thread_data = *((int *)data);

    /*
		kthread_should_stop()返回should_stop标志。它用于创建的线程检查结束标志，并决定
		是否退出。线程完全可以在完成自己的工作后主动结束，不需等待should_stop标志。
    */
    while (1) 
    {
        printk(KERN_INFO "thread_function: %d times data: %d\r\n", time_count, thread_data);
        /*
			void ndelay(unsigned long nsecs);         纳秒级：1/10^-10
			void udelay(unsigned long usecs);         微秒级: 1/10^-6
			void mdelay(unsigned long msecs);         毫秒级：1/10^-3

			msleep：睡眠之后不可唤醒；
			msleep_interuptible：睡眠之后可唤醒；
			ssleep：s延时，睡眠时候不可唤醒；

			虽然msleep和mdelay都有延迟的作用，但他们是有区别的．
			mdeday还忙等待函数（相当于for循环），在延迟过程中无法运行其他任务．
			这个延迟的时间是准确的．是需要等待多少时间就会真正等待多少时间．
			而msleep是休眠函数，它不涉及忙等待．你如果是msleep(10)，那实际上延迟的时间，
			大部分时候是要多于10ms的，是个不定的时间值．

			udelay(); mdelay(); ndelay();实现的原理本质上都是忙等待，ndelay和mdelay
			都是通过udelay衍生出来的
			udelay一般适用于一个比较小的delay，如果你填的数大于2000，系统会认为你这个
			是一个错误的delay函数，因此如果需要2ms以上的delay需要使用mdelay函数。

			由于这些delay函数本质上都是忙等待，对于长时间的忙等待意味这无谓的耗费着cpu的
			资源，因此对于毫秒级的延时，内核提供了msleep，ssleep等函数，这些函数将使得调用
			它的进程睡眠参数指定的时间。
        */
        set_current_state(TASK_UNINTERRUPTIBLE);

        if (kthread_should_stop())
        {
			break;
        }

		/* 条件为真，进行业务处理，条件为假，休眠让出CPU，不退出的原因在于
		   kthread_stop()时由于线程已经退出而引发oops内核异常 */
        if (time_count <= 30)
        {
        	time_count++;
			msleep(1000);
        }
        else
        {
        	/* 线程不退出继续运行 */
        	//msleep(5000);

        	time_count += 5;

        	printk("The thread is about to enter the dormant state. time_count: %d\r\n", time_count);

        	schedule_timeout(5 * HZ);
        }
    };
    
    return time_count;  
}

static int kthreadrun_test_init(void) 
{
	printk(KERN_INFO "kthreadrun_test_init.\n");

	/*
		struct task_struct kthread_run(int (*threadfn)(void *data), void *data, const char namefmt[],...);
		kthread_run()负责内核线程的创建，参数包括入口函数threadfn，参数data，线程名称namefmt。
		可以看到线程的名字可以是类似sprintf方式组成的字符串。
		
	*/
	param_data = 10;
	tsk = kthread_run(thread_function, &param_data, "mythread%d", 1);
	if (IS_ERR(tsk)) 
	{
		printk(KERN_INFO "create kthread failed!\n");
	}
	else 
	{
		printk(KERN_INFO "create ktrhead ok!\n");
	}
	
	return 0;
}

static void kthreadrun_test_exit(void) 
{
	int ret;
	
	printk(KERN_INFO "kthreadrun_test_exit.\n");
	if (!IS_ERR(tsk))
	{
		/*
			kthread_stop()负责结束创建的线程，参数是创建时返回的task_struct指针。
			kthread设置标志should_stop，并等待线程主动结束，返回线程的返回值。线程
			可能在kthread_stop()调用前就结束。（经过实际验证，如果线程在kthread_stop()
			调用之前就结束，之后kthread_stop()再调用会发生可怕地事情—调用kthread_stop()
			的进程crash！！之所以如此，是由于kthread实现上的弊端）
		*/
		ret = kthread_stop(tsk);
		printk(KERN_INFO "thread function has run %ds\n", ret);
	}

	return;
}

MODULE_LICENSE("GPL");  
module_init(kthreadrun_test_init);
module_exit(kthreadrun_test_exit);

#endif

#if KTHREAD_CREATE_TEST
static struct task_struct *task;
static int create_param_data;

static int threadcreate_function(void *data)
{
    int time_count = 0;
    int thread_data = *((int *)data);

    /*
		kthread_should_stop()返回should_stop标志。它用于创建的线程检查结束标志，并决定
		是否退出。线程完全可以在完成自己的工作后主动结束，不需等待should_stop标志。
    */
    while (1) 
    {
        printk(KERN_INFO "threadcreate_function: %d times data: %d\r\n", time_count, thread_data);

        set_current_state(TASK_UNINTERRUPTIBLE);

        if (kthread_should_stop())
        {
			break;
        }

		/* 条件为真，进行业务处理，条件为假，休眠让出CPU，不退出的原因在于
		   kthread_stop()时由于线程已经退出而引发oops内核异常 */
        if (time_count <= 30)
        {
        	time_count++;
			msleep(1000);
        }
        else
        {
        	/* 线程不退出继续运行 */
        	//msleep(5000);

        	time_count += 5;

        	printk("The thread is about to enter the dormant state. time_count: %d\r\n", time_count);

        	schedule_timeout(5 * HZ);
        }
    };
    
    return time_count;  
}

static int kthreadcreate_test_init(void) 
{
    int err;
    
	printk(KERN_INFO "kthreadcreate_test_init.\n");

	/*
		struct task_struct *kthread_create(int (*threadfn)(void *data),void *data,const char *namefmt, ...);
		kthread_create()创建内核线程，参数格式与kthread_run一致。
		区别在于kthread_create()创建内核线程后，不会马上运行，而是需要将kthread_create()返回
		的task_struct指针传给wake_up_process()，软后通过此函数运行线程。		
	*/
	create_param_data = 10;
	task = kthread_create(threadcreate_function, &create_param_data, "mythread%d", 1);
	if (IS_ERR(task)) 
	{
		printk(KERN_INFO "create kthread failed!\n");
		err = PTR_ERR(task);
		task = NULL;
		return err;
	}
	else 
	{
		printk(KERN_INFO "create ktrhead ok!\n");
	}

    //触发内核线程运行
	wake_up_process(task);
	
	return 0;
}

static void kthreadcreate_test_exit(void) 
{
	int ret;
	
	printk(KERN_INFO "kthreadcreate_test_exit.\n");
	if (!IS_ERR(task))
	{
		/*
			kthread_stop()负责结束创建的线程，参数是创建时返回的task_struct指针。
			kthread设置标志should_stop，并等待线程主动结束，返回线程的返回值。线程
			可能在kthread_stop()调用前就结束。（经过实际验证，如果线程在kthread_stop()
			调用之前就结束，之后kthread_stop()再调用会发生可怕地事情—调用kthread_stop()
			的进程crash！！之所以如此，是由于kthread实现上的弊端）
		*/
		ret = kthread_stop(task);
		printk(KERN_INFO "thread function has run %ds\n", ret);
		task = NULL;
	}

	return;
}

MODULE_LICENSE("GPL");  
module_init(kthreadcreate_test_init);
module_exit(kthreadcreate_test_exit);

#endif
