/*
 * Example on how to use POSIX barrier in multi-stage processing.
 *
 * Note that pthread barriers are optional part of POSIX standard and some
 * UNIX systems (e.g. OS X) do not implement them.
 *
 * Compile:
 *   Linux: gcc -pthread pthread-barrier.c
 *   elsewhere: cc pthread-barrier.c
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define	NUM_THREADS	8
#define	NUM_STAGES	3

pthread_barrier_t barrier;

void *
worker(void *x)
{
	int i, cnt;
	int id =  *(int *)x;

	while (1) {
		cnt = random() % 5;
		printf("Thread #%d will loop %d times\n", id, cnt);
		for (i = 0; i < cnt; ++i) {
			(void) printf("thread #%d loop #%d\n", id, i);
			(void) sleep(1);
		}

		printf("thread #%d %p\n", id, &barrier);
		pthread_barrier_wait(&barrier);
	}

	return (NULL);
}

int
main(void)
{
	pthread_t threads[NUM_THREADS];
	int ids[NUM_THREADS];
	pthread_attr_t a;
	int array[NUM_THREADS];
	int i;

	pthread_attr_init(&a);
	pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

	for (i = 0; i < NUM_THREADS; i++) {
		ids[i] = i;
		(void) pthread_create(&threads[i], &a, worker, &ids[i]);
	}

	/* Add 1 for the main thread. */
	pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);

	/* Run one stage after another. */
	for (i = 0; i < NUM_STAGES; i++) {
		printf("Stage %d\n", i);
		sleep(10);
		printf("main %p\n", &barrier);
		pthread_barrier_wait(&barrier);
	}

	pthread_barrier_destroy(&barrier);

	return (0);
}
