/*
 * An example on how to correctly handle signals in a threaded environment. Do
 * not use sigprocmask(3) as noted in the lecture materials (sigprocmask is
 * not defined for threaded environments by the specification).
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define CYCLES	100

sigset_t sigset;

void *thread(void *x)
{
	int i;

	for (i = 0; i < CYCLES; ++i) {
		printf("thread %d (loop #%d)\n", *((int *) x), i);
		sleep(1);
	}
	return NULL;
}

int main(void)
{
	pthread_t t;
	int sig, n1 = 1, n2 = 2;

	sigfillset(&sigset);
	pthread_sigmask(SIG_SETMASK, &sigset, NULL);

	pthread_create(&t, NULL, thread, &n1);
	pthread_create(&t, NULL, thread, &n2);

	while (1) {
		sigwait(&sigset, &sig);
		printf("---> caught signal #%d\n", sig);
	}

	return 0;
}
