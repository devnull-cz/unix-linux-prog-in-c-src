/*
 * This example program shows a correct use of the 1st and 4th parameters in
 * pthread_create(). Compare to pthreads/wrong-use-of-arg.c.
 *
 * (c) jp@devnull.cz
 */


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define	NUM_THREADS	5
#define	NUM_LOOPS	3

void *
thread(void *x)
{
	int i;

	for (i = 0; i < NUM_LOOPS; ++i) {
		printf("Thread %d (loop #%d).\n", *((int *) x), i);
		sleep(1);
	}
	return (NULL);
}

int
main(int argc, char *argv[])
{
	int i, yield = 0;
	int id[NUM_THREADS];
	pthread_t t[NUM_THREADS];

	if (argc > 1) {
		printf("Running with sched_yield().\n");
		yield = 1;
	}

	for (i = 0; i < NUM_THREADS; ++i) {
		id[i] = i;
		pthread_create(&t[i], NULL, thread, id + i);

		if (yield)
			sched_yield();
	}

	/* Avoiding pthread_join() for now. */
	sleep(NUM_LOOPS + 1);

	return (0);
}
