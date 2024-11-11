/*
 * See what happens when writing behind the end of file using mmap'ed segment.
 *
 * Run the program like this and compare:
 *
 *	$ ./a.out 0
 *	$ ./a.out 1
 *	$ ./a.out 2
 */

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>

#define	SEEK_N 100

int
main(int argc, char *argv[])
{
	int fd;
	char c = (char)'a';
	char *addr = NULL;
	enum {
		NO_WRITE = 0,
		WRITE_BEFORE_MMAP,
		WRITE_AFTER_MMAP,
	} mode;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <num>\n", argv[0]);
		exit(1);
	}
	mode = atoi(argv[1]);

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
		err(1, "open");

	if (lseek(fd, SEEK_N, SEEK_SET) == -1)
		err(1, "lseek");

	if (mode == WRITE_BEFORE_MMAP) {
		printf("extending before mmap()\n");
		assert(write(fd, &c, 1) == 1);
	}

	addr = mmap(0, SEEK_N, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
		err(1, "mmap");

	if (mode == WRITE_AFTER_MMAP) {
		printf("extending after mmap()\n");
		assert(write(fd, &c, 1) == 1);
	}

	/* Will crash with mode == 0. */
	addr[SEEK_N - 1] = 0;
	return (0);
}
