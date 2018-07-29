#这是多个源文件编译成内核模块的统一书写方法
ifneq  ($(KERNELRELEASE),)
	obj-m += AtomicTest.o SemTest.o RwSemTest.o SpinLockTest.o RwLockTest.o SeqLockTest.o WaitEventTest.o CompletionTest.o
else
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
endif
