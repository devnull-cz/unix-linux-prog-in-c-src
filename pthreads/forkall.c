/*
 * An example to see that forkall() will replicate all threads. You will need
 * Solaris for that. Maybe other systems support forkall().
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>

void
*thread(void *x)
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

#if !defined (__SVR4) || !defined (__sun) 
	fprintf(stderr, "sorry, this example program needs Solaris\n");
	return(2);
#endif

	pthread_create(&t1, NULL, thread, &n1);
	pthread_create(&t2, NULL, thread, &n2);

	printf("%d: before forkall()\n", getpid());
	pid = forkall();

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
