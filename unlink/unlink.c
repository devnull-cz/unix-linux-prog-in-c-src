/*
 * See what happens when writing to file descriptor of file that was unlinked.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <errno.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	if (argc != 2)
		errx(1, "usage: %s <file>", argv[0]);

	if (creat(argv[1], 0666) == -1)
		err(1, "creat");

	int fd;
	if ((fd = open(argv[1], O_RDWR)) == -1)
		err(1, "open");

	char buf[16];
	printf("writing buffer\n");
	memset(buf, 'A', sizeof (buf));
	write(fd, buf, sizeof (buf));

	printf("unlinking\n");
	if (unlink(argv[1]) == -1)
		err(1, "unlink");

	// The file should not be present on file system anymore.
	struct stat sbuf;
	int r = stat(argv[1], &sbuf);
	assert(r == -1 && errno == ENOENT);

	printf("writing buffer\n");
	printf("wrote %zd bytes\n", write(fd, buf, sizeof (buf)));
	if (lseek(fd, 0, SEEK_SET) == -1)
		err(1, "lseek");

	char readbuf[sizeof (buf) * 4];
	memset(readbuf, '\0', sizeof (readbuf));
	printf("read %zd bytes\n", read(fd, readbuf, sizeof (readbuf)));

	return (0);
}
