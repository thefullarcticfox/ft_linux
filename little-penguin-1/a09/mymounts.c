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
#include <linux/nsproxy.h>	// included for nsproxy
#include <linux/list.h>		// included for list_for_each_entry
#include "mymounts.h"		// included for mnt_namespace, mount

MODULE_LICENSE("GPL");
MODULE_AUTHOR("salec aka thefullarcticfox");
MODULE_DESCRIPTION("Proc module listing mount points for little-penguin-1");
MODULE_VERSION("0.01");

static struct proc_dir_entry *mymounts_file = NULL;
static char *mymounts_data = NULL;

static ssize_t mymounts_read(struct file *fp, char __user *buf,
		size_t len, loff_t *offset)
{
	struct mnt_namespace	*ns = current->nsproxy->mnt_ns;
	struct mount		*mnt;
	const char 		*name;
	const char 		*path;
	char			*tmpbuf;

	tmpbuf = (char*)kmalloc(sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	if (!tmpbuf) {
		printk(KERN_ERR "/proc/mymounts: could not allocate memory\n");
		return -ENOMEM;
	}

	memset(mymounts_data, 0, PAGE_SIZE);

	list_for_each_entry(mnt, &(ns->list), mnt_list) {
		name = "none";
		path = "none";

		if (mnt->mnt_devname) {
			name = mnt->mnt_devname;
		}
		if (mnt->mnt_mountpoint) {
			path = dentry_path_raw(
				mnt->mnt_mountpoint, tmpbuf, PAGE_SIZE);
		}

		// root shows up with name none. fixing this here
		if (strlen(name) == 4 && strlen(path) == 1 &&
			!memcmp(name, "none", 5) && !memcmp(path, "/", 2)) {
			name = "root";
		}

		snprintf(mymounts_data, PAGE_SIZE, "%s%-20s%s\n",
			mymounts_data, name, path);
	}

	kfree(tmpbuf);

	return simple_read_from_buffer(buf, len, offset, mymounts_data,
		strlen(mymounts_data));
}

static struct proc_ops mymounts_ops = {
	.proc_read = mymounts_read
};

static int __init mymounts_init(void)
{
	mymounts_file = proc_create("mymounts", S_IRUGO, NULL, &mymounts_ops);
	if (!mymounts_file) {
		printk(KERN_ERR "/proc/mymounts: proc_create failed\n");
		return -EFAULT;
	}

	mymounts_data = kmalloc(sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	if (!mymounts_data) {
		printk(KERN_ERR "/proc/mymounts: failed to allocate memory\n");
		return -ENOMEM;
	}

	printk(KERN_INFO "/proc/mymounts: created\n");

	return 0;
}

static void __exit mymounts_cleanup(void)
{
	proc_remove(mymounts_file);
	kfree(mymounts_data);
	printk(KERN_INFO "/proc/mymounts: removed\n");
}

module_init(mymounts_init);
module_exit(mymounts_cleanup);
