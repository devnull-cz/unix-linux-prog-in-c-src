/*
 * Demonstrate the behavior of process stack size limit.
 *
 * Compile with:
 *   cc stack-overflow.c
 *
 * The first argument is the max recursion depth.
 *
 * Run with e.g.:
 *   ulimit -s
 *   ./a.out 8000
 *
 * (c) vlada@kotalovi.cz
 */

#define	_XOPEN_SOURCE	700

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <signal.h>

void
recurse(size_t n)
{
	/* Intentionally unused variable to make the stack bigger */
	char foo[1024];

	/* To silence the compiler warning */
	(void) foo;
	printf("%zu ", n);
	fflush(stdout);

	if (n == 0) {
		printf("\n");
		return;
	}

	recurse(--n);
}

int
main(int argc, char *argv[])
{
	if (argc != 2)
		errx(1, "Usage: %s <iterations>", argv[0]);

	/* Should have used strtoull(). */
	recurse(atoi(argv[1]));

	return (0);
}
