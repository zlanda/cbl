#这是多个源文件编译成内核模块的统一书写方法
ifneq  ($(KERNELRELEASE),)
#	obj-m += kthread.o
#	systest-objs := KthreadTest.o
	obj-m += KthreadTest.o
else
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
endif
