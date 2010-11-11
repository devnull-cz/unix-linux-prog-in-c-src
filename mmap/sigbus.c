/*
 * Map a file and show what happens if one accesses data after the mapped chunk.
 * Mapping is done in page granularity so reaching after the mapped chunk is OK
 * as long we access the same page. x86 has 4KB pages.
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void)
{
	int fd;
	char c = (char) 'a';
	char *addr = NULL;

	fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666);

	lseek(fd, 100, SEEK_SET);
	write(fd, &c, 1);

	addr = mmap(0, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	/* On x86, accesing 4095 is OK, 4096 generates SIGSEGV or SIGBUS. */
	//addr[4095] = 0;
	addr[4096] = 0;

	return 0;
}
