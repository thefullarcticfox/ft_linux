MODULENAME	= fortytwo_debugfs
KERNELDIR	= /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS	= -Wall -g

obj-m += $(MODULENAME).o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

install:
	sudo chmod a+rx /sys/kernel/debug
	sudo insmod $(MODULENAME).ko

uninstall:
	sudo rmmod $(MODULENAME)

re: clean all
