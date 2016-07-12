/*
 * Check if mmap'ed memory from a fd can be accessed after the file is removed
 * and the fd is closed.
 *
 * Run like this:
 *   $ cc close.c
 *   $ ./a.out
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/mman.h>

int
main(int argc, char *argv[])
{
	char c;
	int fd, size;
	char *addr;
	int i;

	if (argc != 2)
		errx(1, "usage: %s <file>", argv[0]);

	size = 10;
	fd = open(argv[1], O_RDONLY);
	addr = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

	unlink(argv[1]);
	close(fd);

	printf("%p\n---\n", addr);
	for (i = 0; i < size; i++)
		printf("%c", *(addr + i));
	printf("\n---\n");

	munmap(addr, size);

	return (0);
}
