/*
 * An example on how to deal with signals in the presence of an event loop.  Run
 * the program, then send it SIGINTs via ^C.  To terminate it, you can use
 * SIGQUIT (^\) or ^Z and "kill %<job-number>".
 *
 * (c) vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <stdio.h>
#include <err.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>

volatile sig_atomic_t delivered = 0;

void
sig_handler(int sig)
{
	delivered = 1;
}

int
main(void)
{
	struct sigaction act = { 0 };

	printf("My PID is %d.\n", getpid());

	act.sa_handler = sig_handler;
	sigaction(SIGINT, &act, NULL);

	while (1) {
		printf("Sleeping for 10 seconds.\n");
		/* poll is interruptible */
		if (poll(NULL, 0, 10000) == -1)
			warn("poll");
		if (delivered == 1) {
			printf("Signal delivered, continuing.\n");
			delivered = 0;
		}
	}
}
