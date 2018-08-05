ifneq  ($(KERNELRELEASE),)
	obj-m:=SendPacket.o
else
	KDIR := /lib/modules/3.13.0-32-generic/build
	PWD := $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
endif
