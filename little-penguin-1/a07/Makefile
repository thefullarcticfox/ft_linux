obj-m += fortytwo_debugfs.o

KERNELDIR	= /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS	= -Wall -g

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

install:
	sudo chmod a+rx /sys/kernel/debug
	sudo insmod fortytwo_debugfs.ko

re: clean all
