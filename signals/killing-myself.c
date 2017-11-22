/*
 * Sends itself a SIGTERM signal.
 *
 * (c) jp@devnull.cz
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

	return (0);
}
