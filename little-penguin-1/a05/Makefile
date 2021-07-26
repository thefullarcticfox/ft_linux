MODULENAME = fortytwo

obj-m += $(MODULENAME).o

KERNELDIR	= /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS	= -Wall -g

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

install:
	sudo insmod $(MODULENAME).ko

uninstall:
	sudo rmmod $(MODULENAME)

re: clean all
