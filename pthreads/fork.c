/*
 * This program shows that fork() will replicate only the thread that called
 * it - the main() in this case. See that there are only 2 messages from the
 * child process - when it starts and when it exits; there are no thread
 * "loop" messages that are printed from the parent process.
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>

void *
thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		printf("%d: thread %d (loop #%d)\n", getpid(),
		    *((int *) x), i);
		sleep(1);
	}
	return (NULL);
}

int
main(void)
{
	pid_t pid;
	int n1 = 1;
	int n2 = 2;
	pthread_t t1, t2;

	pthread_create(&t1, NULL, thread, &n1);
	pthread_create(&t2, NULL, thread, &n2);

	printf("%d: main program before fork()\n", getpid());
	pid = fork();

	if (pid == 0) {
		printf("%d: I'm a child, will sleep for a few seconds...\n",
		    getpid());
		sleep(4);
		printf("%d: child exiting\n", getpid());
		return (0);
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return (0);
}
