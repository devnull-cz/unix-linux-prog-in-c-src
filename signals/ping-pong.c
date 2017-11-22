/*
 * Throw SIGTERMs at each other.  Do not loop in the handler.
 */

#define	_XOPEN_SOURCE	700

#include <signal.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

pid_t pid;
char *msg;

#define	PSTR	"I'm parent, killing child.\n"
#define	CSTR	"I'm child, killing parent.\n"

void
handler(int sig)
{
	write(1, msg, strlen(msg));
	sleep(1);
	kill(pid, SIGTERM);
}

int
main(void)
{
	struct sigaction sig = { 0 };

	if ((pid = fork()) == -1)
		err(1, "fork");

	sig.sa_handler = handler;
	if (sigaction(SIGTERM, &sig, NULL) == -1)
		err(1, "sigaction");

	if (pid == 0) {	/* child */
		pid = getppid();
		msg = CSTR;
		while (1)
			pause();
	} else {	/* parent */
		msg = PSTR;
		printf("I'm parent, initiating the killing.\n");
		sleep(1);
		kill(pid, SIGTERM);
		while (1)
			pause();
	}

	return (0);
}
