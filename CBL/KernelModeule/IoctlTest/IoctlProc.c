/*******************************************************************************
*   文  件 名：IoctlProc.c
*   功     能：实现内核Ioctl Proc文件读写功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-25
*   修改历史：无
*******************************************************************************/
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "IoctlProc.h"

#define FOO_TEST_SUPP                       1
#define EOO_TEST_SUPP                       1

struct proc_dir_entry *myproc_dir;

#if FOO_TEST_SUPP
static char *foo_str = NULL;

/*******************************************************************************
* seq_operations->show
* 必须返回0，否则什么也显示不出来
*******************************************************************************/
static int my_proc_foo_show(struct seq_file *m, void *v)
{
    seq_printf(m, "current kernel time is %ld\n", jiffies);
    seq_printf(m, "str is %s\n", foo_str);

    return 0;
}

/*******************************************************************************
* file_operations->open
*******************************************************************************/
static int foo_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, my_proc_foo_show, NULL);
}

/*******************************************************************************
* file_operations->write
*******************************************************************************/
static ssize_t foo_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
    char *tmp = kzalloc((count+1), GFP_KERNEL);
    if (!tmp)
        return -ENOMEM;

    if (copy_from_user(tmp,buffer,count))
    {
        kfree(tmp);
        return -EFAULT;
    }

    kfree(foo_str);
    foo_str = tmp;

    return count;
}

static struct file_operations footest_fops = {
    .owner  = THIS_MODULE,
    .open   = foo_proc_open,           //读
    .release = single_release,
    .read   = seq_read,
    .llseek = seq_lseek,
    .write  = foo_proc_write,          //写
};
#endif

#if EOO_TEST_SUPP
static char *eoo_str = NULL;

/*******************************************************************************
* seq_operations->show
* 必须返回0，否则什么也显示不出来
*******************************************************************************/
static int my_proc_eoo_show(struct seq_file *m, void *v)
{
    seq_printf(m, "current kernel time is %ld\n", jiffies);
    seq_printf(m, "str is %s\n", eoo_str);

    return 0;
}

/*******************************************************************************
* file_operations->open
*******************************************************************************/
static int eoo_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, my_proc_eoo_show, NULL);
}

/*******************************************************************************
* file_operations->write
*******************************************************************************/
static ssize_t eoo_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
    char *tmp = kzalloc((count+1), GFP_KERNEL);
    if (!tmp)
        return -ENOMEM;

    if (copy_from_user(tmp,buffer,count))
    {
        kfree(tmp);
        return -EFAULT;
    }

    kfree(eoo_str);
    eoo_str = tmp;

    return count;
}

static struct file_operations eootest_fops = {
    .owner  = THIS_MODULE,
    .open   = eoo_proc_open,           //读
    .release = single_release,
    .read   = seq_read,
    .llseek = seq_lseek,
    .write  = eoo_proc_write,          //写
};
#endif

int ioctlproc_init(void)
{
    struct proc_dir_entry* file;
    /* 在/proc下创建proc_test目录 */
    myproc_dir = proc_mkdir("proc_test", NULL);

    #if FOO_TEST_SUPP
    //创建proc文件并关联file_operations
    //在/proc/proc_test目录下创建foo文件
    file = proc_create("foo", 0666, myproc_dir, &footest_fops);
    if (!file)
    {
    	printk("proc_create failed.\r\n");
        return -ENOMEM;
    }
    #endif

    #if EOO_TEST_SUPP
    //创建proc文件并关联file_operations
    //在/proc/proc_test目录下创建foo文件
    file = proc_create("eoo", 0666, myproc_dir, &eootest_fops);
    if (!file)
    {
    	printk("proc_create failed.\r\n");
        return -ENOMEM;
    }
    #endif

	printk("proc create succ.\r\n");

    return 0;
}

void ioctlproc_exit(void)
{
    #if FOO_TEST_SUPP
    /* 删除/proc/proc_test/foo文件 */
    remove_proc_entry("foo", myproc_dir);
    #endif

    #if EOO_TEST_SUPP
    /* 删除/proc/proc_test/foo文件 */
    remove_proc_entry("eoo", myproc_dir);
    #endif
    
    /* 删除/proc/proc_test目录 */
    remove_proc_entry("proc_test", NULL);
    
    /* 释放内存 */
    kfree(foo_str);

    /* 释放内存 */
    kfree(eoo_str);

    return;
}

