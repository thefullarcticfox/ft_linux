#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("Simple USB keyboard module for little-penguin-1");
MODULE_VERSION("0.01");

static int __init usbkeyboard_init(void)
{
	printk(KERN_INFO "simple usb keyboard module init\n");
	return 0;
}

static void __exit usbkeyboard_cleanup(void)
{
	printk(KERN_INFO "simple usb keyboard module cleanup\n");
}

module_init(usbkeyboard_init);
module_exit(usbkeyboard_cleanup);
