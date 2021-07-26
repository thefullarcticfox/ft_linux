MODULENAME	= main
KERNELDIR	= /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS	= -Wall -g

obj-m += $(MODULENAME).o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

install:
	sudo insmod $(MODULENAME).ko

uninstall:
	sudo rmmod $(MODULENAME)

re: clean all
