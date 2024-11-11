/*
 * Simple demonstration of memory-mapped access to a file.
 *
 * Run like this:
 *   $ echo "0123456789" > numbers.txt
 *   $ cc reverse.c
 *   $ ./a.out
 *   $ cat numbers.txt
 */

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define	FNAME	"numbers.txt"

int
main(void)
{
	char c;
	int fd, size;
	char *addr, *p1, *p2;

	if ((fd = open(FNAME, O_RDWR)) == -1)
		err(1, "open %s", FNAME);

	size = lseek(fd, 0, SEEK_END);
	/*
	 * MAP_SHARED has to be used in order to sync the data to the backing
	 * store (as opposed to the MAP_PRIVATE).
	 */
	p1 = addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	p2 = p1 + size - 1;

	while (p1 < p2) {
		c = *p1;
		*p1++ = *p2;
		*p2-- = c;
	}

	munmap(addr, size);
	close(fd);
	return (0);
}
