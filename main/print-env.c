/*
 * Print all environment variables.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>

/* This variable is defined in libc.so. */
extern char** environ;

int
main(void)
{
	int i = 0;

	while (environ[i] != NULL)
		printf("%s\n", environ[i++]);

	return (0);
}
