/*
 * Basic example on how to construct a pipe.  Note especially the following:
 *
 *   - the write end of the pipe is closed in the consumer
 *     - that is so that all references to that end are closed once the producer
 *       is done so the consumer can get a proper EOF and not hang indefinitely.
 *
 *   - the read end of the pipe is closed in the producer
 *     - that is so if the consumer ends up prematurely the producer will get a
 *       SIGPIPE to indicate a lack of readers (otherwise it would fill the pipe
 *       buffer and block indefinitely and that would not be acceptable).
 *
 *   - a pipe is constructed from a producer to a consumer (data flow - wise)
 *
 * The program will run /bin/ls in the child and pipe its output to the parent
 * which exec's less(1) on the input from its pipe end.
 */

#define	_XOPEN_SOURCE 700	// needed for the pid_t definition

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int
main(void)
{
	pid_t pid;
	int pd[2];

	if (pipe(pd) == -1)
		err(1, "pipe");

	switch (pid = fork()) {
	case -1:
		err(1, "fork");
	case 0: /* producer */
		(void) close(1);
		(void) dup(pd[1]);
		/* not before dup() ! */
		(void) close(pd[0]);
		(void) close(pd[1]);
		execl("/bin/ls", "ls", "/", (char *)NULL);
		break;
	default: /* consumer */
		(void) close(0);
		(void) dup(pd[0]);
		(void) close(pd[0]);
		(void) close(pd[1]);
		execl("/usr/bin/less", "less", (char *)NULL);
		break;
	}

	return (0);
}
