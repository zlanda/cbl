/*******************************************************************************
*   文  件 名：IoctlTest.c
*   功     能：实现内核Ioctl功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-25
*   修改历史：无
*******************************************************************************/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include "IoctlTest.h"
#include "IoctlProc.h"
#include "IoctlSysfs.h"
#include "Timer.h"
#include "Workqueue.h"
#include "Tasklet.h"

static int mem_major = MEMDEV_MAJOR;

module_param(mem_major, int, S_IRUGO);

struct mem_dev *mem_devp;           /*设备结构体指针*/

struct cdev cdev;

static struct class *mem_cls;

static struct device *mem_device;

/*文件打开函数*/
int mem_open(struct inode *inode, struct file *filp)
{
    struct mem_dev *dev;

    /*获取次设备号*/
    int num = MINOR(inode->i_rdev);

    printk("mem_open num=%d\r\n", num);

    if (num >= MEMDEV_NR_DEVS)
    {
        return -ENODEV;
    }
    
    dev = &mem_devp[num];

    /*将设备描述结构指针赋值给文件私有数据指针*/
    filp->private_data = dev;

    return 0; 
}

/*文件释放函数*/
int mem_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/*IO操作*/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
int memdev_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
    int err = 0;
    int ret = 0;
    int ioarg = 0;

    /* 检测命令的有效性 */
    if (_IOC_TYPE(cmd) != MEMDEV_IOC_MAGIC)
    {
        return -EINVAL;
    }
    
    if (_IOC_NR(cmd) > MEMDEV_IOC_MAXNR)
    {
        return -EINVAL;
    }

    /*
        static inline int access_ok(int type, const void *addr, unsigned long size)
            type :是VERIFY_READ 或者VERIFY_WRITE用来表明是读用户内存还是写用户内存;
            addr:是要操作的用户内存地址;
            size:是操作的长度。如果ioctl需要从用户空间读一个整数，那么size参数就等于sizeof(int)；

            返回值：Access_ok返回一个布尔值：1，是成功（存取没问题）；0，是失败，ioctl返回-EFAULT；
    */
    /* 根据命令类型，检测参数空间是否可以访问 */
    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    }
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    }
    
    if (err)
    {
        return -EFAULT;
    }

    /* 根据命令，执行相应的操作 */
    switch(cmd)
    {
        /* 打印当前设备信息 */
        case MEMDEV_IOCPRINT:
        {
            printk("<--- CMD MEMDEV_IOCPRINT Done--->\n\n");
            break;
        }

        /* 获取参数 */
        case MEMDEV_IOCGETDATA:
        {
            ioarg = 1101;
            ret = __put_user(ioarg, (int *)arg);
            break;
        }

        /* 设置参数 */
        case MEMDEV_IOCSETDATA:
        {
            ret = __get_user(ioarg, (int *)arg);
            printk("<--- In Kernel MEMDEV_IOCSETDATA ioarg = %d --->\n\n",ioarg);
            break;
        }

        default:
        {
            return -EINVAL;
        }
    }
    
    return ret;
}
#else
long memdev_unlocked_ioctl(struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
    int err = 0;
    int ret = 0;
    int ioarg = 0;

    struct inode *inode = file_inode(filp);

    /* 检测命令的有效性 */
    if (_IOC_TYPE(cmd) != MEMDEV_IOC_MAGIC)
    {
        return -EINVAL;
    }
    
    if (_IOC_NR(cmd) > MEMDEV_IOC_MAXNR)
    {
        return -EINVAL;
    }

    /*
        static inline int access_ok(int type, const void *addr, unsigned long size)
            type :是VERIFY_READ 或者VERIFY_WRITE用来表明是读用户内存还是写用户内存;
            addr:是要操作的用户内存地址;
            size:是操作的长度。如果ioctl需要从用户空间读一个整数，那么size参数就等于sizeof(int)；

            返回值：Access_ok返回一个布尔值：1，是成功（存取没问题）；0，是失败，ioctl返回-EFAULT；
    */
    /* 根据命令类型，检测参数空间是否可以访问 */
    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    }
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    }
    
    if (err)
    {
        return -EFAULT;
    }

    /* 根据命令，执行相应的操作 */
    switch(cmd)
    {
        /* 打印当前设备信息 */
        case MEMDEV_IOCPRINT:
        {
            printk("<--- CMD MEMDEV_IOCPRINT Done--->\n\n");
            break;
        }

        /* 获取参数 */
        case MEMDEV_IOCGETDATA:
        {
            ioarg = 1101;
            ret = __put_user(ioarg, (int *)arg);
            break;
        }

        /* 设置参数 */
        case MEMDEV_IOCSETDATA:
        {
            ret = __get_user(ioarg, (int *)arg);
            printk("<--- In Kernel MEMDEV_IOCSETDATA ioarg = %d --->\n\n",ioarg);
            break;
        }

        default:
        {
            return -EINVAL;
        }
    }
    
    return ret;
}

#endif

/*文件操作结构体*/
static const struct file_operations mem_fops =
{
    .owner = THIS_MODULE,
    .open = mem_open,
    .release = mem_release,
    /* 好像必须得这么做，测试结果高版本不用unlocked_ioctl会出现错误 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36)
    .ioctl = memdev_ioctl,
#else
    .unlocked_ioctl  = memdev_unlocked_ioctl,
#endif
};

/*设备驱动模块加载函数*/
static int memdev_init(void)
{
    int result;
    int i;
    dev_t deviceno = 0;
    char devname[16];

    printk("Enter.\r\n");

    dev_t devno = MKDEV(mem_major, 0);

    /*
        内核中所有已分配的字符设备编号都记录在一个名为 chrdevs 散列表里。该散列表中的每一个元素是一个 char_device_struct 结构，它的定义如下：
        static struct char_device_struct {
            struct char_device_struct *next; // 指向散列冲突链表中的下一个元素的指针
            unsigned int major;           // 主设备号
            unsigned int baseminor;       // 起始次设备号
            int minorct;                 // 设备编号的范围大小
            char name[64];        // 处理该设备编号范围内的设备驱动的名称
            struct file_operations *fops;     
            struct cdev *cdev;        // 指向字符设备驱动程序描述符的指针
        } *chrdevs[CHRDEV_MAJOR_HASH_SIZE];

        1.内核并不是为每一个字符设备编号定义一个 char_device_struct 结构，而是为一组对应
          同一个字符设备驱动的设备编号范围定义一个 char_device_struct 结构。chrdevs 散列表
          的大小是 255，散列算法是把每组字符设备编号范围的主设备号以 255 取模插入相应的散
          列桶中。同一个散列桶中的字符设备编号范围是按起始次设备号递增排序的。

        2.注册
          内核提供了三个函数来注册一组字符设备编号，这三个函数分别是 register_chrdev_region()、
          alloc_chrdev_region()和register_chrdev()。这三个函数都会调用一个共用的
          __register_chrdev_region() 函数来注册一组设备编号范围（即一个 char_device_struct 结构）。

          1、int register_chrdev_region(dev_t from, unsigned count, const char *name)
            from :要分配的设备编号范围的初始值(次设备号常设为0);
            Count:连续编号范围.
            name:编号相关联的设备名称. (/proc/devices);

          2、动态分配:
            int alloc_chrdev_region(dev_t *dev,unsigned int firstminor,unsigned int count,char *name);
            firstminor是请求的最小的次编号；
            count是请求的连续设备编号的总数；
            name为设备名，返回值小于0表示分配失败。
            然后通过major=MMOR(dev)获取主设备号

          3、释放:
            void unregist_chrdev_region(dev_t first,unsigned int count);
            调用Documentation/devices.txt中能够找到已分配的设备号．
    */
    /* 静态申请设备号*/
    if (mem_major)
    {
        result = register_chrdev_region(devno, 2, "memdev");
    }
    else  /* 动态分配设备号 */
    {
        result = alloc_chrdev_region(&devno, 0, 2, "memdev");
        mem_major = MAJOR(devno);
    }

    printk("mem_major:%d\r\n", mem_major);

    if (result < 0)
    {
        printk("Can't get device number from the kernel.\r\n");
        return result;
    }

    /*初始化cdev结构*/
    /*
        内核中每个字符设备都对应一个 cdev 结构的变量，一个 cdev 一般它有两种定义初始化方式：静态的和动态的。
            静态内存定义初始化：
            struct cdev my_cdev;
            cdev_init(&my_cdev, &fops);
            my_cdev.owner = THIS_MODULE;

            动态内存定义初始化：
            struct cdev *my_cdev = cdev_alloc();
            my_cdev->ops = &fops;
            my_cdev->owner = THIS_MODULE;
        两种使用方式的功能是一样的，只是使用的内存区不一样，一般视实际的数据结构需求而定。
    */
    cdev_init(&cdev, &mem_fops);
    cdev.owner = THIS_MODULE;
    cdev.ops = &mem_fops;

    /* 注册字符设备 */
    /*
        初始化cdev后，需要把它添加到系统中去。为此可以调用cdev_add()函数。传入cdev结构的指针，
        起始设备编号，以及设备编号范围。
    */
    cdev_add(&cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);

    /* 为设备描述结构分配内存*/
    mem_devp = kmalloc(MEMDEV_NR_DEVS * sizeof(struct mem_dev), GFP_KERNEL);
    if (!mem_devp)    /*申请失败*/
    {
        result = -ENOMEM;
        printk("Can't allocate memory.\r\n");
        goto fail_malloc;
    }
    memset(mem_devp, 0, sizeof(struct mem_dev));

    /*为设备分配内存*/
    for (i = 0; i < MEMDEV_NR_DEVS; i++) 
    {
        mem_devp[i].size = MEMDEV_SIZE;
        mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
        memset(mem_devp[i].data, 0, MEMDEV_SIZE);
    }

    //创建设备节点
    /* 为设备创建结点 */
    mem_cls = class_create(THIS_MODULE, "memdevcls");
    if(IS_ERR(mem_cls))
    {
        cdev_del(&cdev);                                    /*注销设备*/
        kfree(mem_devp);                                    /*释放设备结构体内存*/
        unregister_chrdev_region(MKDEV(mem_major, 0), 2);   /*释放设备号*/
        return -EBUSY;
    }

    //函数在/dev目录下创建相应的设备节点。这样，加载模块的时候，用户空间中的udev会
    //自动响应device_create(…)函数，去/sysfs下寻找对应的类从而创建设备节点。
    for (i = 0; i < MEMDEV_NR_DEVS; i++)
    {
        deviceno = MKDEV(mem_major, i);
        sprintf(devname, "memdev%d", i);
        printk("deviceno:%d devname:%s\r\n", deviceno, devname);
        mem_device = device_create(mem_cls, NULL, deviceno, NULL, devname);//mknod /dev/memdev0
        if(IS_ERR(mem_device))
        {
            class_destroy(mem_cls);
            cdev_del(&cdev);                                    /*注销设备*/
            kfree(mem_devp);                                    /*释放设备结构体内存*/
            unregister_chrdev_region(MKDEV(mem_major, 0), 2);   /*释放设备号*/
            return -EBUSY;
        }
    }

    printk("memdev create succ.\r\n");

    //创建proc文件
    ioctlproc_init();

    //创建sysfs文件
    nuht_sysfs_init();

#if 0
    //创建定时器
    clk_init();
#endif

#if 0
    //启动工作队列
    wq_init();
#endif

    //启动tasklet
    tasklet_demo_init();
    
    printk("memdev_init succ.\r\n");

    return 0;

    fail_malloc: 
    unregister_chrdev_region(devno, 1);

    printk("memdev_init failed.\r\n");
    
    return result;
}

/*模块卸载函数*/
static void memdev_exit(void)
{
    int i = 0;

#if 0
    //定时器销毁
    clk_exit();
#endif

#if 0
    //销毁工作队列
    wq_exit();
#endif

    //tasklet退出
    tasklet_demo_exit();

    ioctlproc_exit();
    nuht_sysfs_exit();
    
    dev_t deviceno = 0;
    for (i = 0; i < MEMDEV_NR_DEVS; i++)
    {
        deviceno = MKDEV(mem_major, i);
        device_destroy(mem_cls, deviceno);
    }
    
    class_destroy(mem_cls);	
    cdev_del(&cdev);                                    /*注销设备*/
    kfree(mem_devp);                                    /*释放设备结构体内存*/
    unregister_chrdev_region(MKDEV(mem_major, 0), 2);   /*释放设备号*/
}

MODULE_AUTHOR("zlanda");
MODULE_LICENSE("GPL");

module_init(memdev_init);
module_exit(memdev_exit);

