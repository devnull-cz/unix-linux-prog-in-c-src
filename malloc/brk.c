/*
 * Compile and run under strace (Linux)/truss (Solaris)/dtruss (Mac OS X)
 * to see the behavior (brk() vs. mmap())
 *
 * gcc brk.c
 * <tracing-tool> ./a.out 1024
 * <tracing-tool> ./a.out 1048576
 * ...
 *
 * vlada@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
	char *s;
	int alloc_size;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <bytecnt>\n", argv[0]);
		exit(1);
	}

	alloc_size = atoi(argv[1]);

	if ((s = (char *) malloc(alloc_size)) == NULL) {
		fprintf(stderr, "malloc failed (%s)\n", strerror(errno));
		exit(1);
	}

	return (0);
}
