/*
 * Pipe reader waits for the last writer to close the pipe.
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	CHILDREN	3

static void
child(int p[2], int num)
{
	pid_t pid;

	if ((pid = fork()) == 0) {
		close(p[0]);
		sleep(num);
		printf("%d exiting\n", getpid());
		_exit(0);
	} else {
		printf("%d forked %d\n", getpid(), pid);
	}
}

int
main(void)
{
	int p[2];
	char buf[1];

	pipe(p);

	for (int i = 0; i < CHILDREN; i++)
		child(p, i + 1);

	close(p[1]);
	printf("%d reading\n", getpid());
	read(p[0], buf, sizeof (buf));
	printf("%d got EOF\n", getpid());

	return (0);
}
