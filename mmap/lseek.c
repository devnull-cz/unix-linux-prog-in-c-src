/*
 * See what happens when writing behind the end of file using mmap'd segment.
 *
 * Run the program like this and compare:
 *  ./a.out 0
 *  ./a.out 1
 *  ./a.out 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int
main(int argc, char *argv[])
{
	int fd;
	char c = (char) 'a';
	char *addr = NULL;
	int mode;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <num>\n", argv[0]);
		exit(1);
	}
	mode = atoi(argv[1]);

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
		perror("open");
		exit(1);
	}

	if (lseek(fd, 100, SEEK_SET) == -1) {
		perror("lseek");
		exit(1);
	}

	if (mode == 1)
		write(fd, &c, 1);

	addr = mmap(0, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((void *) addr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	if (mode == 2)
		write(fd, &c, 1);

	addr[98] = 0;

	return (0);
}
