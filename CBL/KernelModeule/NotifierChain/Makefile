#编译多个内核模块
ifneq  ($(KERNELRELEASE),)
	obj-m += NotifierTest1.o NotifierTest2.o NotifierTest3.o
else
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

endif
	
