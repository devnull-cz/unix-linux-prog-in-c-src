/*
 * Print all strings from argv[].
 */

#include <stdio.h>

int
main(int argc, char **argv)
{
	int i = 0;

	/* You can do it both ways. */
#if 1
	while (argv[i] != NULL)
		printf("[%d] %s\n", i, argv[i++]);
#else
	for (i = 0; i < argc; ++i)
		printf("[%d] %s\n", i, argv[i]);
#endif

	return (0);
}
