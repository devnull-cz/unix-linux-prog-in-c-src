/*
 * Sends itself a SIGTERM signal that is caught.
 *
 * (c) jp@devnull.cz
 */

#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>

#define MESSAGE "TERM signal caught !\n"

void term_handler(int sig)
{
	write(1, MESSAGE, strlen(MESSAGE));
}

int main(void)
{
	struct sigaction act;

	/* we leave flags and mask empty */
	bzero(&act, sizeof(act));
	act.sa_handler = term_handler;
	sigaction(SIGTERM, &act, NULL);

	while (1) {
		printf("sending myself a SIGTERM signal...\n");
		kill(getpid(), SIGTERM);
		sleep(1);
	}

	return (0);
}
