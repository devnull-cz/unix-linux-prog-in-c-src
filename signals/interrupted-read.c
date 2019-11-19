/*
 * An example on an interrupted read(2).  Run the program, then kill it from
 * another terminal.  read(2) should return an error, with EINTR errno.
 *
 *  $ ./a.out
 *  My PID is 30794.
 *  <...send SIGTERM from another terminal>
 *  TERM signal caught !
 *  a.out: read: Interrupted system call
 *
 * Use -r option for SA_RESTART.
 *
 * (c) jp@devnull.cz
 */
#define	_XOPEN_SOURCE	700

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define	MESSAGE "TERM signal caught !\n"

void
term_handler(int sig)
{
	/* write(2) is safe to use from signal handlers. */
	write(1, MESSAGE, sizeof (MESSAGE) - 1);
}

int
main(int argc, char **argv)
{
	char c;
	int fd[2];
	struct sigaction act = { 0 };

	(void)argc;

	printf("My PID is %d (use -r for SA_RESTART).\n", getpid());

	if (argv[1] != NULL && strcmp(argv[1], "-r") == 0)
		act.sa_flags = SA_RESTART;

	act.sa_handler = term_handler;
	sigaction(SIGTERM, &act, NULL);

	if (pipe(fd) == -1)
		err(1, "pipe");
	if (read(fd[0], &c, 1) == -1)
		err(1, "read");
}
