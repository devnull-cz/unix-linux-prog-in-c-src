/*
 * Show the difference between exit() and _exit() w.r.t. atexit().
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h> /* _exit() */
#include <stdlib.h>

void
bye(void)
{
	printf("Taadyadya\n");
}

int
main(void)
{
	int i;

	printf("setting exit handler\n");
	if (atexit(bye) != 0) {
		fprintf(stderr, "cannot set exit function\n");
		exit(EXIT_FAILURE);
	}

/*
 * Remove the comment to get exit().
#define EXIT 1
 */
#ifdef EXIT
	exit(3);
#else
	_exit(4);
#endif
	printf("not reached\n");
}
