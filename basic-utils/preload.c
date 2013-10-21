/*
 * compile:
 *   gcc  -fPIC -c preload.c
 *   gcc -shared -o libpreload.so preload.o
 *
 * run e.g. like this:
 *   LD_PRELOAD=./libpreload.so cat /etc/passwd
 */

#include <unistd.h>
#include <stdio.h>

int
close(int d)
{
	(void) printf("fd %d will not be closed!\n", d);
	return (0);
}
