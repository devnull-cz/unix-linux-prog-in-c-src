/*
 * See what happens with pre-existing anonymous mapping if we map the file
 * into it.
 *
 * Run like this:
 *   gcc override.c
 *   ./a.out 0 0
 *   ./a.out 1 0
 *   ./a.out 1 100
 *
 * vlada@devnull.cz, 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

static void
pmapme(void)
{
	int status;
	char pid_str[16];

	switch (fork()) {
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			snprintf(pid_str, sizeof (pid_str), "%d", getppid());
			execl("/usr/bin/pmap", "pmap", pid_str, NULL);
			break;
		default:
			wait(&status);
			break;
	}
}

static int
create_file(void)
{
	int fd;
	char c = (char) 'a';

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
		perror("open");
		exit(1);
	}

	if (lseek(fd, 99, SEEK_SET) == -1) {
		perror("lseek");
		exit(1);
	}
	write(fd, &c, 1);

	return (fd);
}

int
main(int argc, char *argv[])
{
	int fd;
	char *addr = NULL, *newaddr = NULL;
	int flags = MAP_SHARED;
	off_t off = 0;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <0|1> <addr_offset>\n", argv[0]);
		exit(1);
	}

	printf("original:\n");
	pmapme();

	addr = mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
	    -1, 0);
	printf("anon segment: %p\n", addr);

	pmapme();
	create_file();

	if (atoi(argv[1]) > 0)
		flags |= MAP_FIXED;
	if (atoi(argv[2]) > 0)
		off = atoi(argv[2]);

	newaddr = mmap(addr + off, 100, PROT_READ | PROT_WRITE, flags, fd, 0);
	if ((void *) newaddr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	printf("file segment: wanted: %p got: %p\n", addr, newaddr);

	pmapme();

	newaddr[98] = 0;

	return (0);
}
