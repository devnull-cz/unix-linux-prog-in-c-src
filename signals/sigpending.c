#define	_XOPEN_SOURCE	700

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <err.h>
#include <assert.h>

#define	MESSAGE "SIGINT caught !\n"

void
ctrl_c(int sig)
{
	write(1, MESSAGE, strlen(MESSAGE));
}

int
main(void)
{
	struct sigaction act = { 0 };

	sigset_t sigset;

	/* Block the SIGINT first. */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	/* Install SIGINT handler. */
	act.sa_handler = ctrl_c;
	sigaction(SIGINT, &act, NULL);

	char buf[1];

	printf("Waiting for a char #1\n");
	if (read(1, buf, sizeof (buf)) == -1)
		err(1, "1st read()");

	sigset_t freshset;
	sigemptyset(&freshset); // just to make it obvious
	sigpending(&freshset);
	assert(sigismember(&freshset, SIGINT));

	return (0);
}
