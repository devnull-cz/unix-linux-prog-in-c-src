/*
 * Build like this:
 *
 *   gcc -fPIC -c preload.c
 *   gcc -shared -o libpreload.so preload.o
 *
 * run e.g. like this:
 *   LD_PRELOAD=./libpreload.so cat /etc/passwd
 *
 * Note: this example does not work on OS X
 */

#include <unistd.h>
#include <stdio.h>

/* On Solaris cat(1) calls fclose() which in turn calls _close(). */
int
#ifdef __sun
_close(int d)
#else
close(int d)
#endif
{
	(void) printf("fd %d will not be closed!\n", d);
	return (0);
}
