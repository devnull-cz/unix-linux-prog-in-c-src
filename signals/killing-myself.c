/*
 * This sends itself a SIGTERM signal.
 */

#define	_XOPEN_SOURCE	700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int
main(void)
{
	printf("Sending myself a SIGTERM signal...\n");
	kill(getpid(), SIGTERM);
}
