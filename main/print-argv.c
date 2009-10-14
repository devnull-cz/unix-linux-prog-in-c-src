/*
 * Print all strings from argv[].
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>

int
main(int argc, char **argv)
{
	int i = 0;

	/* You can do it both ways. */
#if 1
	while (argv[i] != NULL)
		printf("%s\n", argv[i++]);
#else
	for (i = 0; i < argc; ++i)
		printf("%s\n", argv[i]);
#endif

	return (0);
}
