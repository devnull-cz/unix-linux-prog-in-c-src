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

int
main(void)
{
	int fd;
	char c = (char) 'a';
	char *addr = NULL;
	int i;

	printf("pagesize = %ld bytes\n", sysconf(_SC_PAGE_SIZE));

	fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666);

	lseek(fd, 100, SEEK_SET);
	write(fd, &c, 1);

	if ((addr = mmap(0, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
	    == NULL) {
		fprintf(stderr, "mmap failed");
		exit(1);
	}

	/*
	 * On Linux 32-bit x86, accesing 0-8191 is OK, 8192 generates
	 * SIGSEGV or SIGBUS.
	 */
	for (i = 4095; i < 16384; i++) {
		printf("%d\n", i); /* use \n so that output is flushed. */
		addr[i] = 0;
	}

	return (0);
}
