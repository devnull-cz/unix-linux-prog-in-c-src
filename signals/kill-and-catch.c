/*
 * Sends itself a SIGTERM signal, repeatedly.  The signal is caught.
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#define	MESSAGE "TERM signal caught !\n"

void
term_handler(int sig)
{
	/* write(2) is safe to use from signal handlers. */
	write(1, MESSAGE, strlen(MESSAGE));
}

int
main(void)
{
	struct sigaction act = { 0 };

	/* We leave flags and mask empty. */
	act.sa_handler = term_handler;
	sigaction(SIGTERM, &act, NULL);

	while (1) {
		printf("Sending myself a SIGTERM signal...\n");
		kill(getpid(), SIGTERM);
		sleep(1);
	}

	return (0);
}
