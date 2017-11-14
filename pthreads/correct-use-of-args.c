/*
 * This example program shows a correct use of the 1st and 4th parameters in
 * pthread_create(). Compare to pthreads/wrong-use-of-arg.c.
 *
 * (c) jp@devnull.cz
 */

#define	_GNU_SOURCE	// for pthread_yield()

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
main(int argc, char *argv[])
{
	pthread_t t[NUM_THREADS];
	int i, yield = 0, id[NUM_THREADS];

	if (argc > 1) {
		printf("running with pthread_yield()\n");
		yield = 1;
	}

	for (i = 0; i < NUM_THREADS; ++i) {
		id[i] = i;
		pthread_create(&t[i], NULL, thread, id + i);

		/*
		 * pthread_yield() is not from POSIX thread API however it is
		 * present on many *nix systems.
		 */
		if (yield)
			pthread_yield();
	}

	/* Avoiding pthread_join() for now. */
	sleep(6);

	return (0);
}
