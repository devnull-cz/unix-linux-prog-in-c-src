/*
 * A pipe reader is blocked on a pipe until all the writers (= children) close
 * their pipe descriptor.
 *
 * Note the parent does not wait() for the children.  Also note that read() is
 * interrupted with the SIGCHLD signal but restarted automatically (you can
 * verify that with strace(1)).
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#define	CHILDREN	3

static void
child(int p[2], int num)
{
	pid_t pid;

	if ((pid = fork()) == 0) {
		close(p[0]);
		sleep(num);
		printf("%d exiting\n", getpid());
		// This will close the write end of the pipe,
		// the unblocking the reader.
		_exit(0);
	}
	printf("%d forked %d\n", getpid(), pid);
}

int
main(void)
{
	int p[2];
	ssize_t n;
	char buf[1];

	pipe(p);

	// Spawn the pseudo-writers.
	for (int i = 0; i < CHILDREN; i++)
		child(p, i + 1);

	close(p[1]);
	printf("%d reading from the pipe\n", getpid());
	n = read(p[0], buf, sizeof (buf));
	assert(n == 0);
	printf("%d got EOF\n", getpid());
}
