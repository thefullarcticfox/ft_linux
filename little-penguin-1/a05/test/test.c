#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define FILE "/dev/fortytwo"

void	printerrno()
{
	printf("errno: %d %s\n", errno, strerror(errno));
	errno = 0;
}

void	readtest(int fd, size_t bytes)
{
	char	buf[bytes + 1];
	ssize_t	retval;

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

int	open_fd(const char *path, int oflag)
{
	int	fd;

	printf(">>> opening %s in mode %d\n", path, oflag);
	if ((fd = open(path, oflag)) < 0) {
		perror("Error");
	}

	return fd;
}

int	main()
{
	int	fd;

	fd = open_fd(FILE, O_RDWR);
	readtest(fd, 5);
	close(fd);

	fd = open_fd(FILE, O_RDWR);
	readtest(fd, 3);
	readtest(fd, 2);
	readtest(fd, 10);
	close(fd);

	fd = open_fd(FILE, O_RDWR);
	readtest(fd, 24);
	writetest(fd, "salec");
	writetest(fd, "sales");
	writetest(fd, "sale");
	writetest(fd, "salec42");
	close(fd);

	return 0;
}
