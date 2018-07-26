/*******************************************************************************
*   文  件 名：IoctlSysfs.c
*   功     能：实现内核Ioctl sysfs文件读写功能
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

#define FOO_ATTR_SUPPORT                        1
#define EOO_ATTR_SUPPORT                        1

static int foo;
static int eoo;

#if FOO_ATTR_SUPPORT
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
	
	.show =foo_show,        //属性显示
	.store =foo_store,      //属性存储
};
#endif

#if EOO_ATTR_SUPPORT
static ssize_t eoo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", eoo);
}

static ssize_t eoo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%du", &eoo);
    return count;
}

static struct kobj_attribute eoo_attribute = /*__ATTR(foo, 0666, foo_show, foo_store);*/
{
	.attr ={
		.name = "eoo",
		.mode = 0666,
	},
	
	.show =eoo_show,        //属性显示
	.store =eoo_store,      //属性存储
};
#endif

static struct attribute *attrs[] = 
{
#if FOO_ATTR_SUPPORT
    &foo_attribute.attr,        //foo属性
#endif
#if EOO_ATTR_SUPPORT
    &eoo_attribute.attr,        //eoo属性
#endif
    NULL,
};

static struct attribute_group attr_group = 
{
    .attrs = attrs,             //属性
};

static struct kobject *foo_kobj;

int nuht_sysfs_init(void)
{
    int retval;

    //在/sys/kernel目录下创建foo_test文件夹
    foo_kobj = kobject_create_and_add("foo_test", kernel_kobj);

    if (!foo_kobj)
        return -ENOMEM;

    //在/sys/kernel/foo_test下创建foo属性文件
    retval = sysfs_create_group(foo_kobj, &attr_group);
    if (retval)
        kobject_put(foo_kobj);

    return retval;
}

void nuht_sysfs_exit(void)
{
    kobject_put(foo_kobj);
}


