/*
 * This example program shows a correct use of the 4th parameter in
 * pthread_create(). Compare to pthreads/wrong-use-of-arg.c.
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define	NUM_THREADS	5

void *
thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		printf("thread %d (loop #%d)\n", *((int *) x), i);
		sleep(1);
	}
	return (NULL);
}

int
main(void)
{
	pthread_t t[NUM_THREADS];
	int i, id[NUM_THREADS];

	for (i = 0; i < NUM_THREADS; ++i) {
		id[i] = i;
		pthread_create(&t[i], NULL, thread, id + i);
#if 0
		pthread_yield();
#endif
	}

	/* avoiding pthread_join() for now */
	sleep(6);

	return (0);
}
