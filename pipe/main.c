/*
 * Basic example on how to construct pipe. Notice especially the following:
 *   - write end of the pipe is closed in consumer
 *     - this is so that all references to it are closed once the producer is
 *       done and so that the consumer can get proper EOF
 *   - read end of the pipe is closed in producer
 *     - this is so that if the consumer ends up prematurely the producer
 *       will get SIGPIPE (otherwise it would fill the pipe buffer and block
 *       indefinitely)
 *   - pipe is constructed from producer to consumer (data flow - wise)
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(void)
{
	int pid;
	int pd[2];

	pipe(pd);

	switch (pid = fork()) {
	case -1:
		fprintf(stderr, "error\n");
		exit(1);
	case 0: /* producer */
		close(1);
		dup(pd[1]);
		// not before dup() !
		close(pd[0]);
		close(pd[1]);
		execl("/bin/ls", "/bin/ls", "/", (char *) NULL);
		break;
	default: /* consumer */
		close(0);
		dup(pd[0]);
		close(pd[0]);
		close(pd[1]);
		execl("/usr/bin/less", "/usr/bin/less", (char *) NULL);
		break;
	}

	return (0);
}
