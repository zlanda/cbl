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

static int kthread_test_init(void) 
{
    printk(KERN_INFO "hello world!\n");

    return 0;
}

static int kthread_test_exit(void) 
{
    printk(KERN_INFO "see you.\n");

    return 0;
}

module_init(kthread_test_init);
module_exit(kthread_test_exit);

