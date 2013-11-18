/*
 * Time limited user input. If a user doesn't input anything in 5 seconds the
 * program complains and exits. Note that read() will return -1 and we print an
 * error message since it was interrupted. Of course, if we used this in
 * production we would hush EINTR.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <err.h>

#define	BUFSIZE	4096
#define	SECS	5

#define MESSAGE "TIMEOUT! Exiting...\n"

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
	struct sigaction act;

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, NULL);
	printf("type anything and hit the enter, you have %d seconds...\n",
	    SECS);
	alarm(SECS);

	if ((size = read(0, buf, BUFSIZE)) == -1) {
#if 0
		/*
		 * That's what we would normally do but let's see that we really
		 * get it.
		 */
		if (errno != EINTR)
#endif
			err(1, "read");
#if 0
		else
			printf("interrupted...\n");
#endif
	}

	if (size > 0) {
		printf("read %d bytes: ", size);
		/* without \n it wouldn't print it before write() */
		fflush(stdout);
		write(1, buf, size);
	}

	return (0);
}
