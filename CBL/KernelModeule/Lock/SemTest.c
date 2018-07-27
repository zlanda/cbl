/*******************************************************************************
*   文  件 名：SemTest.c
*   功     能：测试内核信号量功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-27
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
                             Linux内核锁机制说明
    
*******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sem.h>

struct semaphore sem1;
struct semaphore sem2;

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
	for(i = 0; i < 5; i++)
	{
		down(&sem1);		//获取信号量1
		printk("%d ", num[i]);
		up(&sem2);	//释放信号量2
	}

	return 0;
}

int thread_two(void *p)
{
	int *num = (int *)p;
	int i;

	for(i = 0; i < 5; i++){
		down(&sem2);			   //获取信号量2
		printk("%d ", num[i]);
		up(&sem1);		   //释放信号量1
	}

	return 0;
}

static int lan_init(void)
{
	printk("lan is coming\n");
	init_MUTEX(&sem1);  //初始化信号量1， 使信号量1最初可被获取
	init_MUTEX_LOCKED(&sem2);  //初始化信号量2，使信号量2只有被释放后才可被获取
	kernel_thread(thread_one, num[0], CLONE_KERNEL);
	kernel_thread(thread_two, num[1], CLONE_KERNEL);
	return 0;
}

static void lan_exit(void)
{
	printk("\nlan exit\n");
}

module_init(lan_init);
module_exit(lan_exit);

