/*
 * Show the difference between exit() and _exit() w.r.t. atexit().
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
main(int argc, char **argv)
{
	(void) argv;

	if (argc == 1)
		printf("Run with any command line argument to use exit().\n");

	printf("Setting exit handler.\n");
	if (atexit(bye) != 0) {
		fprintf(stderr, "Cannot set exit function.\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * Use any argument to use exit().
	 */
	if (argc > 1) {
		printf("Using exit().\n");
		exit(3);
	} else {
		printf("Using _exit().\n");
		_exit(4);
	}

	printf("Never reached.\n");
}
