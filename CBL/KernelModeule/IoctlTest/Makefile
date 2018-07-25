ifneq  ($(KERNELRELEASE),)
obj-m := IoctlTest.o
#IoctlTest-objs := IoctlProc.o IoctlTest.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.ko *.o *.symvers *.cmd *.cmd.o *.mod.* *.order .*.*.cmd .tmp*
endif
