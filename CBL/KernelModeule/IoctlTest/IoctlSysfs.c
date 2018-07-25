/*******************************************************************************
*   文  件 名：IoctlSysfs.c
*   功     能：实现内核Ioctl sysfs功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-25
*   修改历史：无
*******************************************************************************/
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

static int foo;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%du", &foo);
    return count;
}

static struct kobj_attribute foo_attribute = /*__ATTR(foo, 0666, foo_show, foo_store);*/
{
	.attr ={
		.name = "foo",
		.mode = 0666,
	},
	
	.show =foo_show,
	.store =foo_store, 
};

static struct attribute *attrs[] = 
{
    &foo_attribute.attr,
    NULL,
};

static struct attribute_group attr_group = 
{
    .attrs = attrs,
};

static struct kobject *nuht_kobj;

int nuht_sysfs_init(void)
{
    int retval;

    nuht_kobj = kobject_create_and_add("nuht_cap", kernel_kobj);

    if (!nuht_kobj)
        return -ENOMEM;

    retval = sysfs_create_group(nuht_kobj, &attr_group);
    if (retval)
        kobject_put(nuht_kobj);

    return retval;
}

void nuht_sysfs_exit(void)
{
    kobject_put(nuht_kobj);
}


