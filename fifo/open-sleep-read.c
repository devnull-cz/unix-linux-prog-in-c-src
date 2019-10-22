/*
 * Create a fifo /tmp/fifo, and run:
 *
 * 	echo hello | cat >/tmp/fifo
 *
 * Then run ./a.out /tmp/fifo to see that the shell command finishes as soon as
 * we start a.out (= we open the file) but we can still read what is in the
 * pipe.
 *
 */
#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int fd, n;
	char buf[128];

	assert(argc == 2);

	if ((fd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open");
	sleep(3);
	printf("Reading now from fd %d.\n", fd);
	if ((n = read(fd, buf, sizeof (buf))) == -1)
		err(1, "read");
	write(1, buf, n);
	printf("Reading again from fd %d.\n", fd);
	n = read(fd, buf, sizeof (buf));
	printf("Read returned %d.\n", n);
}
