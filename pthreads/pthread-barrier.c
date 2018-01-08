/*
 * Example on how to use POSIX barrier in multi-stage processing.
 *
 * Note that pthread barriers are optional part of POSIX standard and some
 * UNIX systems (e.g. macOS) do not implement them.
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
	int i, j, cnt, ret;
	int id = *(int *)x;

	for (j = 0; j < NUM_STAGES; j++) {
		cnt = random() % 5;
		printf("STAGE %d: Thread #%d will loop %d times\n", j, id, cnt);
		for (i = 0; i < cnt; ++i) {
			(void) printf("STAGE %d: thread #%d loop #%d\n",
			    j, id, i);
			(void) sleep(1);
		}

		/* Last thread on the barrier will get special value. */
		printf("STAGE %d: thread #%d reaching barrier.\n", j, id);
		ret = pthread_barrier_wait(&barrier);
		printf("STAGE %d: thread #%d after barrier %s\n", j, id,
		    ret == PTHREAD_BARRIER_SERIAL_THREAD ?
		    "[PTHREAD_BARRIER_SERIAL_THREAD]" : "");
	}

	return (NULL);
}

int
main(void)
{
	pthread_t threads[NUM_THREADS];
	int ids[NUM_THREADS];
	int i;

	pthread_barrier_init(&barrier, NULL, NUM_THREADS);

	for (i = 0; i < NUM_THREADS; i++) {
		ids[i] = i;
		(void) pthread_create(&threads[i], NULL, worker, &ids[i]);
	}

	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	pthread_barrier_destroy(&barrier);

	return (0);
}
