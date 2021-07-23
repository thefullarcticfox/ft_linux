#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO, KERN_ERR
#include <linux/miscdevice.h>	// included for miscdevices
#include <linux/fs.h>		// included for fileoperations
#include <linux/stat.h>		// included for S_IRUGO, S_IWUGO
#include <linux/uaccess.h>	// included for copy_to_user, copy_from_user
#include <linux/errno.h>	// included for EFAULT, EINVAL
#include <linux/string.h>	// included for memcmp

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("A Simple char device module for little-penguin-1");
MODULE_VERSION("0.01");

static ssize_t fortytwo_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset);
static ssize_t fortytwo_write(struct file *fp, const char __user *buf,
		size_t len, loff_t *offset);

static const struct file_operations fortytwo_fops = {
	.owner		= THIS_MODULE,
	.read		= fortytwo_read,
	.write		= fortytwo_write
};

static struct miscdevice fortytwo_dev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "fortytwo",
	.fops		= &fortytwo_fops,
	.mode		= S_IRUGO | S_IWUGO
};

static ssize_t fortytwo_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset)
{
	ssize_t retval;

	if (len == 0) {
		return 0;
	}

	if (len > 5) {
		len = 5;
	}

	retval = copy_to_user(buf, "salec", len);
	if (retval) {
		printk(KERN_ERR "/dev/fortytwo: could not copy %ld bytes to user\n",
			retval);
		return -EFAULT;
	}

	/*
	 * // debug related
	 * printk(KERN_DEBUG "/dev/fortytwo: offset %lld \n", *offset);
	 *
	 * // useful in case i need a device with an end
	 * *offset += len;
	 */

	return len;
}

static ssize_t fortytwo_write(struct file *fp, const char __user *buf,
		size_t len, loff_t *offset)
{
	ssize_t	retval;
	char	tmpbuf[5];

	memset(tmpbuf, 0, 5);

	if (len < 5 || len > 5) {
		return -EINVAL;
	}

	retval = copy_from_user(tmpbuf, buf, 5);
	if (retval) {
		printk(KERN_ERR "/dev/fortytwo: could not copy %ld bytes from user\n",
			retval);
		return -EFAULT;
	}

	if (memcmp(tmpbuf, "salec", 5)) {
		return -EINVAL;
	}

	return len;
}

static int __init fortytwo_init(void)
{
	int retval;

	retval = misc_register(&fortytwo_dev);
	if (retval) {
		printk(KERN_ERR "/dev/fortytwo: failed to register\n");
		return retval;
	}

	printk(KERN_INFO "/dev/fortytwo: successfully registred\n");
	return 0;
}

static void __exit fortytwo_cleanup(void)
{
	misc_deregister(&fortytwo_dev);
	printk(KERN_INFO "/dev/fortytwo: deregistered\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_cleanup);
