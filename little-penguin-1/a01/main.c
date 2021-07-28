// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("Simple Hello World module for little-penguin-1");
MODULE_VERSION("0.01");

static int __init hello_init(void)
{
	pr_info("Hello world!\n");
	return 0;	// Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
	pr_info("Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
