/*
 * An example on how to deal with signals in the presence of event loop.
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <err.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>

volatile sig_atomic_t run = 1;

void
sig_handler(int sig)
{
	run = 0;
}

int
main(void)
{
	struct sigaction act;

	bzero(&act, sizeof (act));
	act.sa_handler = sig_handler;
	sigaction(SIGINT, &act, NULL);

	while (run) {
		printf("sleeping\n");
		if (poll(NULL, 0, 10000) == -1)
			warn("poll");
	}

	printf("cleanup\n");

	return (0);
}
