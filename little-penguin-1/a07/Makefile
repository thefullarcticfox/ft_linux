obj-m += fortytwo_debugfs.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo chmod a+rx /sys/kernel/debug
	sudo insmod fortytwo_debugfs.ko

re: clean all
