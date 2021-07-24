#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO, KERN_ERR
#include <linux/fs.h>		// included for fileoperations
#include <linux/stat.h>		// included for S_IRUGO, S_IWUGO
#include <linux/uaccess.h>	// included for copy_to_user, copy_from_user
#include <linux/errno.h>	// included for EFAULT, ENODEV
#include <linux/string.h>	// included for memcmp, strlen
#include <linux/proc_fs.h>	// included for proc
#include <linux/slab.h>		// included for kmalloc, kfree
#include <linux/rwsem.h>	// included for read write locks

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("Proc module listing mount points for little-penguin-1");
MODULE_VERSION("0.01");

static struct proc_dir_entry *mymounts_file = NULL;

static int mymounts_open(struct inode *node, struct file *fp)
{
	printk(KERN_DEBUG "open handler\n");
	return 0;
}

static ssize_t mymounts_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset)
{
	printk(KERN_DEBUG "read handler\n");
	return 0;
}

static struct proc_ops mymounts_pops = {
	.proc_open = mymounts_open,
	.proc_read = mymounts_read
};

static int __init mymounts_init(void)
{
	mymounts_file = proc_create("mymounts", S_IRUGO, NULL, &mymounts_pops);
	if (!mymounts_file) {
		printk(KERN_ERR "/proc/mymounts: proc_create failed\n");
		return -EFAULT;
	}
	return 0;
}

static void __exit mymounts_cleanup(void)
{
	proc_remove(mymounts_file);
}

module_init(mymounts_init);
module_exit(mymounts_cleanup);
