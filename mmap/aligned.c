/*
 * The program creates a file of the following structure (each size+data chunk
 * is called a record).  The size field is uint32_t.
 *
 * | size | data ... | size | data ... |
 *
 * With 2 records, map it in using mmap() and read it using using pointer
 * arithmetics.
 *
 * Run e.g. like this on x86 and then on SPARC:
 *
 *   ./a.out 16 16
 *   ./a.out 17 16
 *
 * vlada@devnull.cz, 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define	IS_ALIGNED(p) ((((intptr_t)(p)) & 3) == 0)

int
main(int argc, char *argv[])
{
	int fd;
	char c = 'A';
	char *addr = NULL, *eofptr;
	uint32_t *tag;
	uint32_t num;
	int i;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <len1> <len2>\n", argv[0]);
		exit(1);
	}

	size_t rec1_len = atoi(argv[1]), rec2_len = atoi(argv[2]);

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
		perror("open");
		exit(1);
	}

	/* Create the 1st record. */
	num = rec1_len;
	write(fd, &num, sizeof (num));
	for (i = 0; i < rec1_len; i++)
		write(fd, &c, 1);

	/* Create the 2nd record. */
	num = rec2_len;
	write(fd, &num, sizeof (num));
	c++;
	for (i = 0; i < rec2_len; i++)
		write(fd, &c, 1);

	addr = mmap(0, 2 * sizeof (num) + rec1_len + rec2_len,
	    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((void *)addr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	/* One byte past the file end. */
	eofptr = addr + 2 * sizeof (num) + rec1_len + rec2_len;

	/* Read the 1st record. */
	tag = (uint32_t *)addr;
	/* This should be always aligned by the kernel. */
	printf("tag ptr = 0x%p [%s]\n",
		(void *)tag, IS_ALIGNED(tag) ? "aligned" : "not aligned");
	printf("tag = %d\n", *tag);
	/* skip past tag and data */
	addr += sizeof (*tag);
	addr += *tag;

	/* Read the 2nd record. */
	tag = (uint32_t *)addr;
	printf("tag ptr = 0x%p [%s]\n",
		(void *)tag, IS_ALIGNED(tag) ? "aligned" : "not aligned");
	printf("tag = %d\n", *tag);

	/* skip past tag and data */
	addr += sizeof (*tag);
	addr += *tag;

	/* Make sure we have reached end of the file. */
	if (addr != eofptr) {
		fprintf(stderr, "not reached EOF (0x%p != 0x%p)",
		    (void *)addr, (void *)eofptr);
		exit(1);
	}
}
