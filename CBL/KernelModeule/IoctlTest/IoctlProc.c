/*******************************************************************************
*   文  件 名：IoctlProc.c
*   功     能：实现内核Ioctl Proc功能
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

static char *str = NULL;

/*******************************************************************************
* seq_operations->show
* 必须返回0，否则什么也显示不出来
*******************************************************************************/
static int my_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "current kernel time is %ld\n", jiffies);
    seq_printf(m, "str is %s\n", str);

    return 0;
}

/*******************************************************************************
* file_operations->open
*******************************************************************************/
static int ioctltest_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, my_proc_show, NULL);
}

/*******************************************************************************
* file_operations->write
*******************************************************************************/
static ssize_t ioctltest_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
    char *tmp = kzalloc((count+1), GFP_KERNEL);
    if (!tmp)
        return -ENOMEM;

    if (copy_from_user(tmp,buffer,count))
    {
        kfree(tmp);
        return -EFAULT;
    }

    kfree(str);
    str = tmp;

    return count;
}

static struct file_operations ioctltest_fops = {
    .owner  = THIS_MODULE,
    .open   = ioctltest_proc_open,
    .release = single_release,
    .read   = seq_read,
    .llseek = seq_lseek,
    .write  = ioctltest_proc_write,
};

int ioctlproc_init(void)
{
    struct proc_dir_entry* file;

    //创建proc文件并关联file_operations
    file = proc_create("ioctl_test", 0644, NULL, &ioctltest_fops);
    if (!file)
    {
    	printk("proc_create failed.\r\n");
        return -ENOMEM;
    }

	printk("proc create succ.\r\n");

    return 0;
}

void ioctlproc_exit(void)
{
    remove_proc_entry("ioctl_test", NULL);
    kfree(str);
}

