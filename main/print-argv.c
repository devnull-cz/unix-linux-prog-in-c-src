/*
 * Print all strings from argv[].
 */

#include <stdio.h>

int
main(int argc, char **argv)
{
	int i = 0;

	/* You can do it both ways. */

	printf("argv guided:\n");
	while (argv[i] != NULL) {
		printf("[%d] %s\n", i, argv[i]);
		++i;
	}

	printf("argc guided:\n");
	for (i = 0; i < argc; ++i)
		printf("[%d] %s\n", i, argv[i]);

	return (0);
}
