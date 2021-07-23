#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define ID_FILE "/sys/kernel/debug/fortytwo/id"
#define JIFFIES_FILE "/sys/kernel/debug/fortytwo/jiffies"
#define FOO_FILE "/sys/kernel/debug/fortytwo/foo"
#define PAGE_SIZE getpagesize()

void	printerrno() {
	printf("errno: %d %s\n", errno, strerror(errno));
	errno = 0;
}

void	readtest(int fd, size_t bytes)
{
	ssize_t	retval;
	char	buf[PAGE_SIZE + 1];

	printf("> reading %ld bytes\n", bytes);
	retval = read(fd, buf, bytes);

	if (retval < 0) {
		printerrno();
	} else {
		buf[retval] = 0;
		printf("read: %s\nactual bytes read: %ld\n", buf, retval);
	}

	printf("\n");
}

void	writetest(int fd, const char* buf)
{
	ssize_t	retval;

	printf("> writing `%s`\n", buf);
	retval = write(fd, buf, strlen(buf));

	if (retval < 0) {
		printerrno();
	} else {
		printf("successfully wrote %ld bytes\n", retval);
	}

	printf("\n");
}

void	id_test()
{
	int		fd;

	printf("--------------------\n> Testing %s\n", ID_FILE);

	if ((fd = open(ID_FILE, O_RDWR)) < 0) {
		perror("Error");
		return;
	}

	readtest(fd, 5);
	readtest(fd, 3);
	readtest(fd, 2);
	readtest(fd, 24);

	writetest(fd, "salec");
	writetest(fd, "sales");
	writetest(fd, "sale");
	writetest(fd, "salec42");

	close(fd);

}

void	jiffies_test()
{
	int		fd;

	printf("--------------------\n> Testing %s\n", JIFFIES_FILE);

	if ((fd = open(JIFFIES_FILE, O_RDONLY)) < 0) {
		perror("Error");
		return;
	}

	readtest(fd, 42);
	readtest(fd, 42);
	readtest(fd, 42);
	readtest(fd, 42);

	writetest(fd, "salec");

	close(fd);
}

void	foo_root_test()
{
	int		fd;

	printf("--------------------\n> Testing %s (root required to write)\n", FOO_FILE);

	if ((fd = open(FOO_FILE, O_RDWR)) < 0) {
		perror("Error");
		return;
	}

	readtest(fd, 5);
	readtest(fd, 3);
	readtest(fd, 2);
	readtest(fd, 24);

	writetest(fd, "salec");
	writetest(fd, "sales");
	writetest(fd, "sale");
	writetest(fd, "salec42");

	close(fd);
}


void	foo_test()
{
	int		fd;

	printf("--------------------\n> Testing %s\n", FOO_FILE);

	if ((fd = open(FOO_FILE, O_RDONLY)) < 0) {
		perror("Error");
		return;
	}

	readtest(fd, 5);
	readtest(fd, 3);
	readtest(fd, 2);
	readtest(fd, 24);

	close(fd);
}

int		main()
{
	id_test();
	jiffies_test();
	foo_test();
	foo_root_test();
	foo_test();

    return 0;
}
