/*
 * An example to see that forkall() will replicate all threads. You will need
 * Solaris for that. Maybe other systems support forkall().
 *
 * (c) jp@devnull.cz
 */

#include <sys/wait.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>

void *
thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		(void) printf("%d: thread %d (loop #%d)\n", getpid(),
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

#if !defined(__SVR4) || !defined(__sun)
	(void) fprintf(stderr, "Sorry, this example program needs Solaris.\n");
	return (2);
#endif

	pthread_create(&t1, NULL, thread, &n1);
	pthread_create(&t2, NULL, thread, &n2);

	(void) printf("%d: in main before forkall()\n", getpid());
	pid = forkall();

	if (pid == 0) {
		(void) printf("%d: I'm a child, will join my threads...\n",
		    getpid());
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		(void) printf("%d: child joined all its threads.\n", getpid());
		return (0);
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	(void) printf("%d in main: threads joined.\n", getpid());

	/* We can even wait for the child here. */
	if ((pid = wait(NULL)) != -1) {
		(void) printf("%d in main: child %d exited.\n", getpid(), pid);
	} else {
		(void) printf("wait: %s\n", strerror(errno));
		exit(1);
	}

	return (0);
}
