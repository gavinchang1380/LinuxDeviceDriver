# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

ccflags-y += -O2 -I..

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m	:= shortprint.o

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers

depend .depend dep:
	$(CC) $(ccflags-y) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
