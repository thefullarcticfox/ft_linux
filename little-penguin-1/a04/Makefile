obj-m += simple_usb_keyboard_module.o

KERNELDIR	= /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS	= -Wall -g

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

re: clean all
