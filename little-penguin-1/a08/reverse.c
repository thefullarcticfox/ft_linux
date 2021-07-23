#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/rwsem.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Reverse module");

DECLARE_RWSEM(reverse_lock);
static char str[PAGE_SIZE + 1];

static ssize_t reverse_read(struct file *fp, char __user *user,
		size_t size, loff_t *offset);
static ssize_t reverse_write(struct file *fp, const char __user *user,
		size_t size, loff_t *offset);

static struct file_operations reverse_fops = {
	.owner		= THIS_MODULE,
	.read		= &reverse_read,
	.write		= &reverse_write
};

static struct miscdevice reverse_device = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "reverse",
	.fops		= &reverse_fops,
	.mode		= S_IRUGO | S_IWUGO
};

static ssize_t reverse_read(struct file *fp, char __user *user,
		size_t size, loff_t *offset)
{
	int	t;
	int	i;
	char	*tmp;
	ssize_t	retval;

	tmp = (char*)kmalloc(sizeof(char) * (PAGE_SIZE + 1), GFP_KERNEL);
	if (!tmp) {
		return -ENOMEM;
	}

	down_read(&reverse_lock);
	for (t = strlen(str) - 1, i = 0; t >= 0; t--, i++) {
		tmp[i] = str[t];
	}
	up_read(&reverse_lock);
	tmp[i] = 0x0;

	retval = simple_read_from_buffer(user, size, offset, tmp, strlen(tmp));
	kfree(tmp);

	return retval;
}

static ssize_t reverse_write(struct file *fp, const char __user *user,
		size_t size, loff_t *offset)
{
	ssize_t retval = 0;

	down_write(&reverse_lock);
	retval = simple_write_to_buffer(str, size, offset, user, PAGE_SIZE);
	str[retval] = 0x0;
	up_write(&reverse_lock);

	return retval;
}

static int __init reverse_init(void)
{
	int retval;

	retval = misc_register(&reverse_device);
	memset(str, 0, PAGE_SIZE + 1);

	return retval;
}

static void __exit reverse_cleanup(void)
{
	misc_deregister(&reverse_device);
}

module_init(reverse_init);
module_exit(reverse_cleanup);
