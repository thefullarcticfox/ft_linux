#include <linux/init.h>		// included for __init and __exit macros
#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO, KERN_ERR
#include <linux/fs.h>		// included for fileoperations
#include <linux/stat.h>		// included for S_IRUGO, S_IWUGO
#include <linux/uaccess.h>	// included for copy_to_user, copy_from_user
#include <linux/errno.h>	// included for EFAULT, ENODEV
#include <linux/string.h>	// included for memcmp, strlen
#include <linux/debugfs.h>	// included for debufs
#include <linux/jiffies.h>	// included for jiffies
#include <linux/slab.h>		// included for kmalloc, kfree
#include <linux/rwsem.h>	// included for read write locks

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("A Simple debugfs module for little-penguin-1");
MODULE_VERSION("0.01");

static struct dentry *fortytwo_dir = NULL;

DECLARE_RWSEM(foo_lock);
static char *foo_data = NULL;

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

	if (len == 0) {
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
	u64	timer;
	ssize_t retval;
	char	str[64];
	size_t	sz;

	if (len <= 0) {
		return 0;
	}

	timer = get_jiffies_64();
	snprintf(str, sizeof(str), "%lld", timer);
	sz = strlen(str);

	if (len > sz) {
		len = sz;
	}

	retval = copy_to_user(buf, str, len);
	if (retval) {
		printk(KERN_ERR "/debugfs/fortytwo/jiffies: could not copy %ld bytes\n",
			retval);
		return -EFAULT;
	}

	return len;
}

static ssize_t foo_file_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset)
{
	ssize_t retval;

	if (*offset + len >= PAGE_SIZE) {
		len = PAGE_SIZE - *offset;
	}

	if (len == 0) {
		return 0;
	}

	/* *************** *
	 * READ LOCK BEGIN *
	 * *************** */
	down_read(&foo_lock);

	// additional check for data length (EOF)
	len = min(len, strlen(foo_data + *offset));

	retval = copy_to_user(buf, foo_data + *offset, len);
	*offset += len - retval;

	up_read(&foo_lock);
	/* *************** *
	 *  READ LOCK END  *
	 * *************** */

	if (retval) {
		printk(KERN_ERR "/debugfs/fortytwo/foo: could not copy %ld bytes to user\n",
			retval);
		return -EFAULT;
	}

	printk(KERN_DEBUG "/debugfs/fortytwo/foo: read success: %ld bytes\n", len);
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

	/* **************** *
	 * WRITE LOCK BEGIN *
	 * **************** */
	down_write(&foo_lock);

	if (*offset == 0) {
		memset(foo_data, 0, PAGE_SIZE);
	}

	if (*offset + len >= PAGE_SIZE) {
		len = PAGE_SIZE - *offset;
	}

	retval = copy_from_user(foo_data + *offset, buf, len);
	*offset += len - retval;

	up_write(&foo_lock);
	/* **************** *
	 *  WRITE LOCK END  *
	 * **************** */

	if (retval) {
		printk(KERN_ERR "/debugfs/fortytwo/foo: could not copy %ld bytes from user\n",
			retval);
		return -EFAULT;
	}

	printk(KERN_DEBUG "/debugfs/fortytwo/foo: write success: %ld bytes\n", len);
	return len;
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
		"foo", S_IRUGO | S_IWUSR, fortytwo_dir, NULL, &foo_file_fops
	);
	if (!foo_file || foo_file == ERR_PTR(-ENODEV)) {
		printk(KERN_ERR "/debugfs/fortytwo/foo: failed to create file\n");
		return -ENODEV;
	}

	/*
	 * Allocating memory for foo file data
	 * PAGE_SIZE is 4096 most of the time + 1 byte for EOF
	 * GFP_KERNEL - Allocate normal kernel ram. May sleep.
	 */
	foo_data = kmalloc(PAGE_SIZE + 1, GFP_KERNEL);
	if (!foo_data) {
		printk(KERN_ERR "/debugfs/fortytwo/foo: could not allocate memory\n");
		return -ENOMEM;
	}

	memset(foo_data, 0, PAGE_SIZE + 1);

	printk(KERN_INFO "/debugfs/fortytwo: created\n");
	printk(KERN_INFO "/debugfs/fortytwo: pagesize is %ld\n", PAGE_SIZE);
	return 0;
}

static void __exit fortytwo_cleanup(void)
{
	kfree(foo_data);
	debugfs_remove_recursive(fortytwo_dir);
	printk(KERN_INFO "/debugfs/fortytwo: removed\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_cleanup);
