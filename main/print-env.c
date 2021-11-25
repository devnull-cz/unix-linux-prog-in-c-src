/*
 * Print all environment variables.
 */
#include <stdio.h>

/* This variable is defined in libc.so. */
extern char **environ;

int
main(void)
{
	for (int i = 0; environ[i] != NULL; ++i)
		printf("%s\n", environ[i]);
}
