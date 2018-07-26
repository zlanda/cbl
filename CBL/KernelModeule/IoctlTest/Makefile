#这是多个源文件编译成内核模块的统一书写方法
ifneq  ($(KERNELRELEASE),)
	#m表示编译的是一个模块，y表示将其编译进内核
	#注意：当只有一个源文件的时候，只需要obj-m += obj-m += systest.o就可以了
	#当有多个源文件时，则必须使用：模块名-objs来指定模块的依赖文件，并且此时模块名是可以任意取名的
	obj-m += systest.o
	systest-objs := IoctlTest.o IoctlProc.o Timer.o Workqueue.o Tasklet.o IoctlSysfs.o
else
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
endif
