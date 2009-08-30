#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void)
{
	char c;
	int fd, size;
	char *addr, *p1, *p2;

	fd = open("numbers.txt", O_RDWR);
	size = lseek(fd, 0, SEEK_END);
	p1 = addr = mmap(0, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

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
