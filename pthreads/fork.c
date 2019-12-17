/*
 * This program shows that fork() will replicate only the thread that called
 * it - the main() in this case.  Note that there are only 2 messages from the
 * child process - when it starts and when it exits; there are no thread "loop"
 * messages that are printed from the parent process.
 *
 * (c) jp@devnull.cz
 */

#include <err.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define	NUM_LOOP	5
#define	NSECS		4

void *
thread(void *x)
{
	for (int i = 0; i < NUM_LOOP; ++i) {
		printf("%d: thread %d (loop #%d).\n", getpid(),
		    *((int *)x), i);
		sleep(1);
	}
	return (NULL);
}

int
main(void)
{
	pid_t pid;
	pthread_t t1, t2;
	int e, n1 = 1, n2 = 2;

	(void) pthread_create(&t1, NULL, thread, &n1);
	(void) pthread_create(&t2, NULL, thread, &n2);

	printf("%d: main program before fork()\n", getpid());
	sleep(1);

	if ((pid = fork()) == -1)
		err(1, "fork");

	if (pid == 0) {
		printf("%d: child main() sleeping for %d seconds...\n",
		    getpid(), NSECS);
		sleep(NSECS);
		printf("%d: child exiting from main()\n", getpid());
		return (0);
	}

	if ((e = pthread_join(t1, NULL)) != 0)
		warnx("pthread_join: %s", strerror(e));
	if ((e = pthread_join(t2, NULL)) != 0)
		warnx("pthread_join: %s", strerror(e));

	return (0);
}
