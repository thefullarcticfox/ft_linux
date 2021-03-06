// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO, KERN_ERR
#include <linux/fs.h>		// included for fileoperations
#include <linux/stat.h>		// included for S_IRUGO, S_IWUGO
#include <linux/uaccess.h>	// included for copy_to_user, copy_from_user
#include <linux/errno.h>	// included for EFAULT, ENODEV
#include <linux/string.h>	// included for memcmp, strlen
#include <linux/debugfs.h>	// included for debugfs
#include <linux/jiffies.h>	// included for jiffies
#include <linux/slab.h>		// included for kmalloc, kfree
#include <linux/rwsem.h>	// included for read write locks

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("Simple debugfs module for little-penguin-1");
MODULE_VERSION("0.01");

static struct dentry *fortytwo_dir;
static const char *student_id = "salec";

DECLARE_RWSEM(foo_lock);
static char *foo_data;

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
	return simple_read_from_buffer(buf, len, offset, student_id,
		strlen(student_id));
}

static ssize_t id_file_write(struct file *fp, const char __user *buf,
			     size_t len, loff_t *offset)
{
	ssize_t	retval;
	char	tmpbuf[5];

	memset(tmpbuf, 0, 5);

	if (len < 5 || len > 5)
		return -EINVAL;

	retval = copy_from_user(tmpbuf, buf, 5);
	if (retval) {
		pr_err("/dev/fortytwo: could not copy %ld bytes from user\n",
		       retval);
		return -EFAULT;
	}

	if (memcmp(tmpbuf, student_id, 5))
		return -EINVAL;

	return len;
}

static ssize_t jiffies_file_read(struct file *fp, char __user *buf,
				 size_t len, loff_t *offset)
{
	char	str[64];

	snprintf(str, sizeof(str), "%lld\n", get_jiffies_64());
	return simple_read_from_buffer(buf, len, offset, str, strlen(str));
}

static ssize_t foo_file_read(struct file *fp, char __user *buf,
			     size_t len, loff_t *offset)
{
	ssize_t retval;

	if (*offset + len >= PAGE_SIZE)
		len = PAGE_SIZE - *offset;

	if (len == 0)
		return 0;

	down_read(&foo_lock);

	// additional check for data length (EOF)
	len = min(len, strlen(foo_data + *offset));
	retval = copy_to_user(buf, foo_data + *offset, len);
	*offset += len - retval;

	up_read(&foo_lock);

	if (retval) {
		pr_err("/debugfs/fortytwo/foo: could not copy %ld bytes to user\n",
		       retval);
		return -EFAULT;
	}

	pr_debug("/debugfs/fortytwo/foo: read success: %ld bytes\n", len);
	return len;
}

/*
 * thoughts on possible behaviours: read and write ops have common *offset
 * If i read and then write to any common file it will write starting from
 * position of the last read which is standard file behaviour.
 * Here writing to this file allowed only for root user, so two variants:
 * 1. automatically 'lseek' to 0 byte on write
 *    (requires to write all bytes at once)
 * 2. leave this as a standard file behaviour
 *    (as it doesn't matter - those behaviours bound to fd)
 */

static ssize_t foo_file_write(struct file *fp, const char __user *buf,
			      size_t len, loff_t *offset)
{
	ssize_t	retval;

	down_write(&foo_lock);

	if (*offset == 0)
		memset(foo_data, 0, PAGE_SIZE);

	if (*offset + len >= PAGE_SIZE)
		len = PAGE_SIZE - *offset;

	retval = copy_from_user(foo_data + *offset, buf, len);
	*offset += len - retval;

	up_write(&foo_lock);

	if (retval) {
		pr_err("/debugfs/fortytwo/foo: could not copy %ld bytes from user\n",
		       retval);
		return -EFAULT;
	}

	pr_debug("/debugfs/fortytwo/foo: write success: %ld bytes\n", len);
	return len;
}

static int __init fortytwo_init(void)
{
	struct dentry *id_file;
	struct dentry *jiffies_file;
	struct dentry *foo_file;

	fortytwo_dir = debugfs_create_dir("fortytwo", NULL);
	if (!fortytwo_dir || fortytwo_dir == ERR_PTR(-ENODEV)) {
		pr_err("/debugfs/fortytwo: failed to create dir\n");
		return -ENODEV;
	}

	// S_IRUGO | S_IWUGO is 0666
	id_file = debugfs_create_file
		("id", 0666, fortytwo_dir, NULL, &id_file_fops);
	if (!id_file || id_file == ERR_PTR(-ENODEV)) {
		pr_err("/debugfs/fortytwo/id: failed to create file\n");
		return -ENODEV;
	}

	// S_IRUGO is 0444
	jiffies_file = debugfs_create_file
		("jiffies", 0444, fortytwo_dir, NULL, &jiffies_file_fops);
	if (!jiffies_file || jiffies_file == ERR_PTR(-ENODEV)) {
		pr_err("/debugfs/fortytwo/jiffies: failed to create file\n");
		return -ENODEV;
	}

	// S_IRUGO | S_IWUSR is 0644
	foo_file = debugfs_create_file
		("foo", 0644, fortytwo_dir, NULL, &foo_file_fops);
	if (!foo_file || foo_file == ERR_PTR(-ENODEV)) {
		pr_err("/debugfs/fortytwo/foo: failed to create file\n");
		return -ENODEV;
	}

	/*
	 * Allocating memory for foo file data
	 * PAGE_SIZE is 4096 most of the time + 1 byte for EOF
	 * GFP_KERNEL - Allocate normal kernel ram. May sleep.
	 */
	foo_data = kmalloc(sizeof(char) * (PAGE_SIZE + 1), GFP_KERNEL);
	if (!foo_data)
		return -ENOMEM;

	memset(foo_data, 0, PAGE_SIZE + 1);

	pr_info("/debugfs/fortytwo: created\n");
	pr_info("/debugfs/fortytwo: pagesize is %ld\n", PAGE_SIZE);
	return 0;
}

static void __exit fortytwo_cleanup(void)
{
	kfree(foo_data);
	debugfs_remove_recursive(fortytwo_dir);
	pr_info("/debugfs/fortytwo: removed\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_cleanup);
