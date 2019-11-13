/*
 * The code demonstrates that a process writing to pipe without readers will get
 * a SIGPIPE signal.
 *
 * Note that a shell provides a return value of a command killed by a signal as
 * 128+<signal-number>.  In case of a SIGPIPE, the signal number is 13 (run
 * "kill -l SIGPIPE" to verify) so the return value of this program will be
 * 128+13=141.
 *
 * Run like this:
 *   ./a.out
 *   ret=$?
 *   echo $ret
 *   kill -l $(expr $ret - 128)
 *
 * Note that some kill(1) implementations will decrement the 128 value
 * automatically:
 *
 *  $ kill -l 13
 *  PIPE
 *  $ kill -l 141
 *  PIPE
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void)
{
	int pd[2];

	if (pipe(pd) == -1)
		err(1, "pipe");

	/* No reader exists after this. */
	(void) close(pd[0]);

	/* This will fail as we are writing to a pipe without a reader. */
	if (write(pd[1], "", 1) == -1)
		err(1, "write");

	return (0);
}
