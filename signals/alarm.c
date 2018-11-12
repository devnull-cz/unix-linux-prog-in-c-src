/*
 * Time limited user input.  If a user does not enter anything within 5 seconds
 * the program complains and bails out.  Note that read() will return -1 and
 * an error message is printed since the call was interrupted.  Of course, if we
 * used this in production we would not announce an interrupted call but
 * continued.
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700	// for sigaction()

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>

#define	BUFSIZE	4096
#define	SECS	5

#define	MESSAGE	"TIMEOUT!  Exiting...\n"

void
handler(int sig)
{
	write(1, MESSAGE, strlen(MESSAGE));
}

int
main(void)
{
	int size;
	char buf[BUFSIZE];
	struct sigaction act = { 0 };

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);

	sigaction(SIGALRM, &act, NULL);
	printf("Type anything and hit enter, you have %d seconds...\n", SECS);
	alarm(SECS);

	if ((size = read(0, buf, BUFSIZE)) == -1) {
		if (errno != EINTR)
			err(1, "read");
		printf("Interrupted read()...\n");
	}

	/* Disable the repeating timer. */
	alarm(0);
	if (size > 0) {
		printf("Read %d bytes: ", size);
		/* Without \n it would not print it before write() */
		fflush(stdout);
		write(1, buf, size);
	}

	return (0);
}
