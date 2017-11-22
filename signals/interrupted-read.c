/*
 * An example on an interrupted read(2).  Run the program, then send it SIGTERM
 * from another terminal.  read(2) should return an error, with EINTR errno:
 *
 *  $ ./a.out
 *  My PID is 30794.
 *  <...send SIGTERM from another terminal>
 *  TERM signal caught !
 *  a.out: read: Interrupted system call
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

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
	char c;
	int ret, fd[2];
	struct sigaction act = { 0 };

	printf("My PID is %d.\n", getpid());

	act.sa_handler = term_handler;
	sigaction(SIGTERM, &act, NULL);

	if (pipe(fd) == -1)
		err(1, "pipe");
	ret = read(fd[0], &c, 1);
	if (ret == -1)
		err(1, "read");
	return (0);
}
