#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("A Simple char device module for little-penguin-1");
MODULE_VERSION("0.01");

static int __init fortytwo_init(void)
{
	printk(KERN_INFO "Initiating /dev/fortytwo\n");
	return 0;
}

static void __exit fortytwo_cleanup(void)
{
	printk(KERN_INFO "Cleaning up /dev/fortytwo\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_cleanup);
