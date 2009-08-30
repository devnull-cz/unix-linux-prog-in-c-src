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

	while (argv[i] != NULL)
		printf("%s\n", argv[i++]);

	return (0);
}
