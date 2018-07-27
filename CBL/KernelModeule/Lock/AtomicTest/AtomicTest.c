/*******************************************************************************
*   文  件 名：AtomicTest.c
*   功     能：测试内核原子锁功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-27
*   修改历史：无
*******************************************************************************/

//https://blog.csdn.net/godleading/article/details/78259842
//http://www.cnblogs.com/alantu2018/p/8447487.html

/*******************************************************************************
                             Linux内核锁机制说明
    所谓原子操作，就是该操作绝不会在执行完毕前被任何其他任务或事件打断，也就是说，它是
最小的执行单位，不可能有比它更小的执行单位，因此这里的原子实际是使用了物理学里的物质微粒
的概念。
    原子操作需要硬件的支持，因此是架构相关的，其API和原子类型的定义都定义在内核源码树
的include/asm/atomic.h文件中，他们都使用汇编语言实现，因为C语言并不能实现这样的操作。
    原子操作主要用于实现资源计数，很多引用计数（refcnt）就是通过原子操作实现的。

    原子操作API：
    1、原子类型定义如下：
    typedef struct {
        volatile int counter;
    } atomic_t;
    volatile修饰字段告诉gcc不能对该类型的数据做优化处理，对它的访问都是对内存的访问，而
不是对寄存器的访问。

    2、atomic_read(atomic_t * v);
    该函数对原子类型的变量进行原子读操作，它返回原子类型的变量v的值。

    3、atomic_set(atomic_t * v, int i);
    该函数设置原子类型的变量v的值为i。

    4、void atomic_add(int i, atomic_t *v);
    该函数给原子类型的变量v增加值i。

    5、atomic_sub(int i, atomic_t *v);
    该函数从原子类型的变量v中减去i。

    6、int atomic_sub_and_test(int i, atomic_t *v);
    该函数从原子类型的变量v中减去i，并判断结果是否为0，如果为0，返回真，否则返回假。

    7、void atomic_inc(atomic_t *v);
    该函数对原子类型变量v原子地增加1。

    8、void atomic_dec(atomic_t *v);
    该函数对原子类型的变量v原子地减1。

    9、int atomic_dec_and_test(atomic_t *v);
    该函数对原子类型的变量v原子地减1，并判断结果是否为0，如果为0，返回真，否则返回假。

    10、int atomic_inc_and_test(atomic_t *v);
    该函数对原子类型的变量v原子地增加1，并判断结果是否为0，如果为0，返回真，否则返回假。

    11、int atomic_add_negative(int i, atomic_t *v);
    该函数对原子类型的变量v原子地增加i，并判断结果是否为负数，如果是，返回真，否则返回假。

    12、int atomic_add_return(int i, atomic_t *v);
    该函数对原子类型的变量v原子地增加i，并且返回指向v的指针。

    13、int atomic_sub_return(int i, atomic_t *v);
    该函数从原子类型的变量v中减去i，并且返回指向v的指针。

    14、int atomic_inc_return(atomic_t * v);
    该函数对原子类型的变量v原子地增加1并且返回指向v的指针。

    15、int atomic_dec_return(atomic_t * v);
    该函数对原子类型的变量v原子地减1并且返回指向v的指针。

    原子操作通常用于实现资源的引用计数，在TCP/IP协议栈的IP碎片处理中，就使用了引用计数，碎片
队列结构struct ipq描述了一个IP碎片，字段refcnt就是引用计数器，它的类型为atomic_t，当创建IP碎片
时（在函数ip_frag_create中），使用atomic_set函数把它设置为1，当引用该IP碎片时，就使用函数
atomic_inc把引用计数加1，当不需要引用该IP碎片时，就使用函数ipq_put来释放该IP碎片，ipq_put
使用函数atomic_dec_and_test把引用计数减1并判断引用计数是否为0，如果是就释放IP碎片。
函数ipq_kill把IP碎片从ipq队列中删除，并把该删除的IP碎片的引用计数减1（通过使用函数atomic_dec实现）。

*******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/timer.h>
#include <linux/delay.h>

static atomic_t iCount;
static struct timer_list clk_timer;
static struct timer_list clk_timer1;
static struct timer_list clk_timer2;


static void clk_timer_do(unsigned long l)
{
    atomic_inc(&iCount);
    printk("iCount Inc0: %d\r\n", iCount);
    
    mod_timer(&clk_timer, jiffies + HZ);    //修改定时器过期时间
}

static void clk_timer_do1(unsigned long l)
{
    atomic_inc(&iCount);
    printk("iCount Inc1: %d\r\n", iCount);
    
    mod_timer(&clk_timer, jiffies + 2 * HZ);    //修改定时器过期时间
}

static void clk_timer_do2(unsigned long l)
{
    atomic_dec(&iCount);
    printk("iCount Dec: %d\r\n", iCount);
    
    mod_timer(&clk_timer, jiffies + HZ);    //修改定时器过期时间
}
   
static int atomic_test_init(void) 
{
    printk(KERN_INFO "atomic_test_init.\n");

    /* 初始化原子量 */
    atomic_set(&iCount, 10);
    
    /* 创建定时器 */
    init_timer(&clk_timer);             //初始化定时器
    clk_timer.expires = jiffies + HZ;   //1s,过期时间
    clk_timer.function = clk_timer_do;  //设置定时器到期执行函数
    clk_timer.data = 0;
    add_timer(&clk_timer);              //添加定时器到内核

    /* 创建定时器 */
    init_timer(&clk_timer1);             //初始化定时器
    clk_timer1.expires = jiffies + HZ;   //1s,过期时间
    clk_timer1.function = clk_timer_do1;  //设置定时器到期执行函数
    clk_timer1.data = 0;
    add_timer(&clk_timer1);              //添加定时器到内核

    /* 创建定时器 */
    init_timer(&clk_timer2);             //初始化定时器
    clk_timer2.expires = jiffies + HZ;   //1s,过期时间
    clk_timer2.function = clk_timer_do2;  //设置定时器到期执行函数
    clk_timer2.data = 0;
    add_timer(&clk_timer2);              //添加定时器到内核

    return 0;
}

static int atomic_test_exit(void) 
{
    printk(KERN_INFO "atomic_test_exit.\n");

    del_timer(&clk_timer);              //删除定时器
    del_timer(&clk_timer1);              //删除定时器
    del_timer(&clk_timer2);              //删除定时器

    return 0;
}

module_init(atomic_test_init);
module_exit(atomic_test_exit);

