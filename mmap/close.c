/*
 * Check if mmap'ed memory from a fd can be accessed after the fd is closed.
 *
 * Run like this:
 *   $ cc reverse.c
 *   $ ./a.out
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int
main(void)
{
	char c;
	int fd, size;
	char *addr;
	int i;

	size = 10;
	fd = open("/etc/passwd", O_RDONLY);
	addr = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

	close(fd);

	printf("%p\n---\n", addr);
	for (i = 0; i < size; i++)
		printf("%c", *(addr + i));
	printf("\n---\n");

	munmap(addr, size);

	return (0);
}
