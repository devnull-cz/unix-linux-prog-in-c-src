/*
 * This example program shows incorrect use of the 1st and 4th parameters in
 * pthread_create().
 *
 * The thread data structure needs to be unique per thread, otherwise the
 * thread will become unjoinable.
 *
 * Note that the thread function uses the memory where the counter is located,
 * not the counter value as was defined on the main()'s stack.  What you are
 * going to see is implementation dependent, you can use "thread 5" messages
 * only, or some initial messages with other thread ID's before "i" gets to 5.
 * Also, try to run with any argument to see what happens.
 *
 * Compare to correct-use-of-arg.c.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

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
	int i, j, e, yield = 0;
	pthread_t t;
	void *p;

	if (argc > 1) {
		printf("Running with sched_yield().\n");
		yield = 1;
	}

	for (i = 0; i < NUM_THREADS; ++i) {
		/* Note we are forgetting the created threads. */
		pthread_create(&t, NULL, thread, &i);
		if (yield)
			sched_yield();
	}

	/* Let the threads run for some time before waiting on them. */
	sleep(NUM_LOOPS);

	for (j = 0; j < NUM_THREADS; ++j) {
		printf("Joining thread (%d)", j);
		/* We are joining the last thread multiple times. */
		if ((e = pthread_join(t, &p)) != 0)
			printf(": error: %s\n", strerror(e));
		else
			printf(".\n");
	}

	return (0);
}
