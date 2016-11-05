ifneq ($(KERNELRELEASE),)
	obj-m := notifier.o notified.o
else
	KDIR := /lib/modules/`uname -r`/build
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif

