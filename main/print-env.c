/*
 * Print all environment variables.
 */
#include <stdio.h>

/* This variable is defined in libc.so. */
extern char **environ;

int
main(void)
{
	for (char **ep = environ; *ep != NULL; ++ep)
		printf("%s\n", *ep);
}
