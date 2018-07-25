/*******************************************************************************
*   文  件 名：Cdev.c
*   功     能：实现内核Cdev功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-25
*   修改历史：无
*******************************************************************************/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
 
static int major = 250;
static int minor=0;
static dev_t devno;
static struct class *cls;
static struct device *test_device;

static int chardev_open (struct inode *inode, struct file *filep)
{
    printk("chardev_open \n");
    return 0;
}

static struct file_operations chardev_ops=
{
    .open = chardev_open,
};

static int chardev_init(void)
{
    int ret;
    
    printk("chardev_init \n");

    devno = MKDEV(major,minor);
    //向内核注册一个字符设备
    //第一个参数是主设备号，0代表动态分配，这里major是250
    //第二个参数是设备的名字
    //第三个参数是文件操作指针
    //每个设备文件对应有两个设备号：一个主设备号，标识该设备的种类，也标识了该设备所
    //使用的驱动程序；另一个是次设备号，标识使用同一设备驱动程序的不同硬件设备。
    //设备文件的主设备号与设备驱动程序在登录该设备时申请的主设备号移植，否则用户进程将
    //无法访问到设备驱动程序。
    ret = register_chrdev(major, "chardev", &chardev_ops);

    //在/sys/class/上创建myclass目录
    //class_create动态创建设备的逻辑类，并完成部分字段的初始化，然后将其添加到内核中。
    //创建的逻辑类位于/sys/class/。
    //参数：
    //      owner, 拥有者。一般赋值为THIS_MODULE。
    //      name, 创建的逻辑类的名称。
    cls = class_create(THIS_MODULE, "myclass");
    if(IS_ERR(cls))
    {
        unregister_chrdev(major,"chardev");
        return -EBUSY;
    }

    //创建设备节点
    //函数在/dev目录下创建相应的设备节点。这样，加载模块的时候，用户空间中的udev会
    //自动响应device_create(…)函数，去/sysfs下寻找对应的类从而创建设备节点。
    test_device = device_create(cls, NULL, devno, NULL, "chardev");//mknod /dev/chardev
    if(IS_ERR(test_device))
    {
        class_destroy(cls);
        unregister_chrdev(major,"chardev");
        return -EBUSY;
    }

    return 0;
}

static int chardev_exit(void)
{
    device_destroy(cls,devno);
    class_destroy(cls);	
    unregister_chrdev(major,"chardev");
    printk("chardev_exit \n");

    return 0;
}

MODULE_LICENSE("GPL");
module_init(chardev_init);
module_exit(chardev_exit);

