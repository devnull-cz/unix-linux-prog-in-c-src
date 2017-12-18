/*
 * If you need to pass just a thread ID only, you can use the void pointer
 * itself. However, this way is defined as not portable in the specification
 * so be careful. Compare to wrong-use-of-arg.c and correct-use-of-arg.c.
 *
 * Compile as:
 *
 *   cc -pthread int-as-arg.c
 *
 * (c) jp@devnull.cz
 */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <sys/types.h>

#define	NUM_THREADS	5
#define	NUM_LOOPS	3

void *
thread(void *x)
{
	int i;

	for (i = 0; i < NUM_LOOPS; ++i) {
		printf("Thread %d (loop #%d).\n", (int)x, i);
		sleep(1);
	}
	return (NULL);
}

int
main(void)
{
	int i;
	pthread_t t[NUM_THREADS];

	/* Let's not shoot our leg off. */
	assert(sizeof (int) <= sizeof (void *));

	for (i = 0; i < NUM_THREADS; ++i) {
		pthread_create(&t[i], NULL, thread, (void *)(intptr_t)i);
	}

	/* Avoid pthread_join() for now. */
	sleep(NUM_LOOPS + 1);

	return (0);
}
