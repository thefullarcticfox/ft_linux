#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define ID_FILE "/sys/kernel/debug/fortytwo/id"
#define JIFFIES_FILE "/sys/kernel/debug/fortytwo/jiffies"
#define FOO_FILE "/sys/kernel/debug/fortytwo/foo"
#define PAGE_SIZE getpagesize()

void	printstat(const char *path)
{
	struct stat file_stat;

	if (stat(path, &file_stat) < 0)
		return;

	printf("%-34s : ", path);
	printf( (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
	printf( (file_stat.st_mode & S_IRUSR) ? "r" : "-");
	printf( (file_stat.st_mode & S_IWUSR) ? "w" : "-");
	printf( (file_stat.st_mode & S_IXUSR) ? "x" : "-");
	printf( (file_stat.st_mode & S_IRGRP) ? "r" : "-");
	printf( (file_stat.st_mode & S_IWGRP) ? "w" : "-");
	printf( (file_stat.st_mode & S_IXGRP) ? "x" : "-");
	printf( (file_stat.st_mode & S_IROTH) ? "r" : "-");
	printf( (file_stat.st_mode & S_IWOTH) ? "w" : "-");
	printf( (file_stat.st_mode & S_IXOTH) ? "x" : "-");
	printf("\n");
}

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

void	id_test()
{
	int	fd;

	printf(">>>>> Testing %s\n", ID_FILE);

	fd = open_fd(ID_FILE, O_RDWR);
	readtest(fd, 5);
	close(fd);

	fd = open_fd(ID_FILE, O_RDWR);
	readtest(fd, 3);
	readtest(fd, 2);
	readtest(fd, 10);
	close(fd);

	fd = open_fd(ID_FILE, O_RDWR);
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

	printf(">>>>> Testing %s\n", JIFFIES_FILE);

	fd = open_fd(JIFFIES_FILE, O_RDONLY);
	readtest(fd, 5);
	readtest(fd, 3);
	readtest(fd, 11);
	close(fd);

	fd = open_fd(JIFFIES_FILE, O_RDONLY);
	readtest(fd, 42);
	close(fd);
}

void	foo_root_test()
{
	int		fd;

	printf(">>>>> Testing %s (root required to write)\n", FOO_FILE);

	fd = open_fd(FOO_FILE, O_RDWR);
	if (fd < 0) return;

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

	printf(">>>>> Testing %s\n", FOO_FILE);

	fd = open_fd(FOO_FILE, O_RDONLY);

	readtest(fd, 5);
	readtest(fd, 3);
	readtest(fd, 2);
	readtest(fd, 24);

	close(fd);
}

int		main()
{
	printf("----------------------------------------------------------------------\n");
	id_test();
	printf("----------------------------------------------------------------------\n");
	jiffies_test();
	printf("----------------------------------------------------------------------\n");
	foo_test();
	printf("----------------------------------------------------------------------\n");
	foo_root_test();
	printf("----------------------------------------------------------------------\n");
	foo_test();
	printf("----------------------------------------------------------------------\n");

	printf(">>>>> Permissions:\n");
	printstat(ID_FILE);
	printstat(JIFFIES_FILE);
	printstat(FOO_FILE);

	return 0;
}
