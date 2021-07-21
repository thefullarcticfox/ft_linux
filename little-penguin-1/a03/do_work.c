#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

int do_work(int *my_int, int retval)
{
	int x;
	int y;
	int z;

	y = *my_int;
	for (x = 0; x < y; ++x) {
		udelay(10);
	}

	if (y < 10) {
		/* That was a long sleep, tell userspace about it */
		pr_info("We slept a long time!");
	}

	z = x * y;

	/* return 1; */
	return z;
}

int work_init(void)
{
	int x = 10;

	x = do_work(&x, x);

	return x;
}

void work_exit(void)
{
}

module_init(work_init);
module_exit(work_exit);