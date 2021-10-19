/*
 * If you need to debug your code and get a core dump right before exit (e.g. to
 * look for memory leaks), you can use atexit() with abort().
 */

#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	printf("setting exit handler to abort()\n");
	if (atexit(abort) != 0) {
		fprintf(stderr, "cannot set exit function\n");
		exit(EXIT_FAILURE);
	}
}
