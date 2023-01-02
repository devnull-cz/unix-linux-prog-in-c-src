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
 * This is a fixed version of implement-barrier.c.
 *
 * To propertly test the solution, use it something like this: "./a.out 30 0".
 * If the printed column is not perfectly aligned, there is a problem.
 */

/* For srandom() and random(). */
#define	_XOPEN_SOURCE	700

#include <assert.h>
#include <err.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int wokenup;
int max = 20;
int on_barrier;
int maxrand = 50;

void *
thread(void *arg)
{
	while (1) {
		if (maxrand > 0)
			poll(NULL, 0, (random() % maxrand) * 10);

		printf("%d ", *(int *)arg);
		fflush(stdout);

		pthread_mutex_lock(&mutex);
		on_barrier++;

		if ((on_barrier % max) == 0) {
			printf("\n");
			wokenup = 1;
			pthread_cond_broadcast(&cond);
			/*
			 * You need the second barrier so that we do not start
			 * incrementing on_barrier before all the threads got
			 * out of the waiting loop after the first barrier was
			 * reached.  As the first barrier loop uses the modulo
			 * result as its condition, if you incremented
			 * on_barrier before that, any thread still in
			 * pthread_cond_wait(&cond, &m) would never get out of
			 * the loop.
			 */
			while (wokenup < max)
				pthread_cond_wait(&cond, &mutex);
		} else {
			while ((on_barrier % max) != 0)
				pthread_cond_wait(&cond, &mutex);
			++wokenup;
			if (wokenup == max) {
				pthread_cond_broadcast(&cond);
			} else {
				/*
				 * See above on why we need the 2nd barrier.
				 */
				while (wokenup < max)
					pthread_cond_wait(&cond, &mutex);
			}
		}
		pthread_mutex_unlock(&mutex);
	}

	/* Not reached. */
	return (NULL);
}

int
main(int argc, char **argv)
{
	int i, *id;
	pthread_t *tid;

	if (argc > 1)
		max = atoi(argv[1]);
	if (argc > 2)
		maxrand = atoi(argv[2]);

	if (max < 2)
		errx(1, "The argument must be bigger than 1.");

	tid = malloc(sizeof (tid) * max);
	assert(tid != NULL);
	id = malloc(sizeof (id) * max);
	assert(id != NULL);
	for (i = 0; i < max; ++i)
		id[i] = i;

	srandom(time(NULL));

	for (i = 0; i < max; ++i)
		(void) pthread_create(tid + i, NULL, thread, id + i);

	/* Never reached.  We should destroy the cv and the mutex here. */
	for (i = 0; i < max; ++i)
		(void) pthread_join(*(tid + i), NULL);

	return (0);
}
