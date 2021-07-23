#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO, KERN_ERR
#include <linux/fs.h>		// included for fileoperations
#include <linux/stat.h>		// included for S_IRUGO, S_IWUGO
#include <linux/uaccess.h>	// included for copy_to_user, copy_from_user
#include <linux/errno.h>	// included for EFAULT, ENODEV
#include <linux/string.h>	// included for memcmp
#include <linux/debugfs.h>	// included for debufs
#include <linux/jiffies.h>	// included for jiffies

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("A Simple debugfs module for little-penguin-1");
MODULE_VERSION("0.01");

static struct dentry *fortytwo_dir = NULL;

static ssize_t id_file_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset);
static ssize_t id_file_write(struct file *fp, const char __user *buf,
		size_t len, loff_t *offset);
static ssize_t jiffies_file_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset);
static ssize_t foo_file_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset);
static ssize_t foo_file_write(struct file *fp, const char __user *buf,
		size_t len, loff_t *offset);

static const struct file_operations id_file_fops = {
	.owner		= THIS_MODULE,
	.read		= id_file_read,
	.write		= id_file_write
};

static const struct file_operations jiffies_file_fops = {
	.owner		= THIS_MODULE,
	.read		= jiffies_file_read
};

static const struct file_operations foo_file_fops = {
	.owner		= THIS_MODULE,
	.read		= foo_file_read,
	.write		= foo_file_write
};

static ssize_t id_file_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset)
{
	ssize_t retval;

	if (len <= 0) {
		return 0;
	}

	if (len > 5) {
		len = 5;
	}

	retval = copy_to_user(buf, "salec", len);
	if (retval) {
		printk(KERN_ERR "/dev/fortytwo: could not copy %ld bytes\n",
			retval);
		return -EFAULT;
	}

	return len;
}

static ssize_t id_file_write(struct file *fp, const char __user *buf,
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
		return -EFAULT;
	}

	if (memcmp(tmpbuf, "salec", 5)) {
		return -EINVAL;
	}

	return len;
}

static ssize_t jiffies_file_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset)
{
	return 0;
}

static ssize_t foo_file_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset)
{
	return 0;
}


static ssize_t foo_file_write(struct file *fp, const char __user *buf,
		size_t len, loff_t *offset)
{
	return 0;
}

static int __init fortytwo_init(void)
{
	struct dentry *id_file;
	struct dentry *jiffies_file;
	struct dentry *foo_file;

	fortytwo_dir = debugfs_create_dir("fortytwo", NULL);
	if (!fortytwo_dir || fortytwo_dir == ERR_PTR(-ENODEV)) {
		printk(KERN_ERR "/debugfs/fortytwo: failed to create dir\n");
		return -ENODEV;
	}

	id_file = debugfs_create_file(
		"id", S_IRUGO | S_IWUGO, fortytwo_dir, NULL, &id_file_fops
	);
	if (!id_file || id_file == ERR_PTR(-ENODEV)) {
		printk(KERN_ERR "/debugfs/fortytwo/id: failed to create file\n");
		return -ENODEV;
	}

	jiffies_file = debugfs_create_file(
		"jiffies", S_IRUGO, fortytwo_dir, NULL, &jiffies_file_fops
	);
	if (!jiffies_file || jiffies_file == ERR_PTR(-ENODEV)) {
		printk(KERN_ERR "/debugfs/fortytwo/jiffies: failed to create file\n");
		return -ENODEV;
	}

	foo_file = debugfs_create_file(
		"foo", S_IRUGO | S_IWUGO, fortytwo_dir, NULL, &foo_file_fops
	);
	if (!foo_file || foo_file == ERR_PTR(-ENODEV)) {
		printk(KERN_ERR "/debugfs/fortytwo/foo: failed to create file\n");
		return -ENODEV;
	}

	return 0;
}

static void __exit fortytwo_cleanup(void)
{
	debugfs_remove_recursive(fortytwo_dir);
	printk(KERN_INFO "/debugfs/fortytwo: removed\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_cleanup);
