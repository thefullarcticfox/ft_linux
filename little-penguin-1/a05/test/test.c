#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

void	printerrno() {
	printf("errno: %d %s\n", errno, strerror(errno));
	errno = 0;
}

void	readtest(int fd, size_t bytes)
{
	ssize_t	retval;
	char	buf[42];

	printf("> reading %ld bytes\n", bytes);
	retval = read(fd, buf, bytes);

	if (retval < 0) {
		printerrno();
	} else {
		buf[retval] = 0;
		printf("read: %s\n", buf);
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

int		main()
{
	int		fd;

	if ((fd = open("/dev/fortytwo", O_RDWR)) < 0) {
		perror("Error");
		return 1;
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

    return 0;
}
