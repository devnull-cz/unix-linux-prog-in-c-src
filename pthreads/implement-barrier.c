/*
 * A bunch of threads simulate some work with a random sleep, then each prints
 * its ID, and waits for all other threads reaching the barrier before carrying
 * on with the same thing.
 *
 * Be default, it will print something like the following.  Each line represents
 * one barrier.
 *
 *	$ ./a.out
 *	5 4 2 3 8 0 9 1 6 7
 *	9 6 5 7 0 8 2 4 1 3
 *	1 0 7 6 3 5 8 9 2 4
 *	6 9 8 1 4 2 3 7 5 0
 *	2 7 9 8 4 6 3 1 5 0
 *	...
 *
 * However, if you let it run for a while, it will deadlock itself.
 *
 * Can you find the problem?  The solution is in implement-barrier-fixed.c but
 * go find the fix on your own.
 *
 * To quickly deadlock, comment out the sleep call.
 */

/* For srandom() and random(). */
#define	_XOPEN_SOURCE	700

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#define	MAX_WAIT	10

int queue, max;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int max = 10;
int on_barrier;

void *
th(void *arg)
{
	while (1) {
		sleep(random() % MAX_WAIT);

		printf("%d ", *(int *)arg);
		fflush(stdout);

		pthread_mutex_lock(&mutex);
		on_barrier++;

		if ((on_barrier % max) == 0) {
			printf("\n");
			pthread_cond_broadcast(&cond);
		} else {
			while ((on_barrier % max) != 0)
				pthread_cond_wait(&cond, &mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
}

int
main(int argc, char **argv)
{
	int i, *id;
	pthread_t *tid;

	if (argc == 2)
		max = atoi(argv[1]);

	tid = malloc(sizeof (tid) * max);
	assert(tid != NULL);
	id = malloc(sizeof (id) * max);
	assert(id != NULL);
	for (i = 0; i < max; ++i)
		id[i] = i;

	srandom(time(NULL));

	for (i = 0; i < max; ++i)
		(void) pthread_create(tid + i, NULL, th, id + i);

	/* Never reached.  We should destroy the cv and the mutex here. */
	for (i = 0; i < max; ++i)
		(void) pthread_join(*(tid + i), NULL);

	return (0);
}
