/*
 * Example on how to use POSIX barrier in multi-stage processing when
 * the threads are kept around until the program exits. Another possibility
 * would be to let the threads exit after each stage and recreate them.
 *
 * Vladimir Kotal, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define	NUM_THREADS	8
#define	NUM_STAGES	3

pthread_barrier_t barrier1, barrier2;

void *
worker(void *x)
{
	int i, cnt;
	int stage = 0;
	int id =  *(int *)x;

	while (1) {
		cnt = random() % 5;
		printf("Thread #%d will loop %d times\n", id, cnt);
		for (i = 0; i < cnt; ++i) {
			(void) printf("thread #%d loop #%d\n", id, i);
			(void) sleep(1);
		}

		if (stage % 2 == 0) {
			printf("thread #%d %p\n", id, &barrier1);
			pthread_barrier_wait(&barrier1);
		} else {
			printf("thread #%d %p\n", id, &barrier2);
			pthread_barrier_wait(&barrier2);
		}
		stage++;
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
	pthread_barrier_init(&barrier1, NULL, NUM_THREADS + 1);
	pthread_barrier_init(&barrier2, NULL, NUM_THREADS + 1);

	/* Run one stage after another. */
	for (i = 0; i < NUM_STAGES; i++) {
		printf("Stage %d\n", i);
		sleep(10);
		if (i % 2 == 0) {
			printf("main %p\n", &barrier1);
			pthread_barrier_wait(&barrier1);
		} else {
			printf("main %p\n", &barrier2);
			pthread_barrier_wait(&barrier2);
		}
	}

	pthread_barrier_destroy(&barrier1);
	pthread_barrier_destroy(&barrier2);

	return (0);
}
