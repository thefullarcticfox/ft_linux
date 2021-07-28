// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

int do_work(int y)
{
	int x;

	for (x = 0; x < y; ++x)
		usleep_range(9, 11);

	if (y < 10) {
		/* That was a long sleep, tell userspace about it */
		pr_info("We slept a long time!");
	}

	return x * y;
}

int work_init(void)
{
	int x = 10;

	x = do_work(x);

	return x;
}

void work_exit(void)
{
}

module_init(work_init);
module_exit(work_exit);
