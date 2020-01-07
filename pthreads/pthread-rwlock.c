/*
 * See if RW lock implementation is writer biased.
 */

#define	_XOPEN_SOURCE   700

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <err.h>
#include <string.h>
#include <errno.h>

#define	MSEC2NSEC(n)	((n) * 1000 * 1000)

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

pthread_mutex_t rcount_mutex = PTHREAD_MUTEX_INITIALIZER;
size_t rwaitcount;
pthread_mutex_t wcount_mutex = PTHREAD_MUTEX_INITIALIZER;
size_t wcount;

void *
thread(void *arg)
{
	int i, e;
	struct timespec tspec;
	bool writer = (bool)arg;

	(void) printf("thread %12d is %s\n", (int)pthread_self(),
	    writer ? "writer" : "reader");

	for (i = 0; i < 5; ++i) {
		(void) printf("thread %12d loop #%d\n", (int)pthread_self(), i);

		if (writer) {
			pthread_rwlock_wrlock(&rwlock);

			printf("writer!\n");
			pthread_mutex_lock(&wcount_mutex);
			wcount++;
			pthread_mutex_unlock(&wcount_mutex);
		} else {
			while ((e = pthread_rwlock_tryrdlock(&rwlock)) != 0) {
				if ((e == EBUSY) || (e == ETIMEDOUT)) {
					pthread_mutex_lock(&rcount_mutex);
					rwaitcount++;
					pthread_mutex_unlock(&rcount_mutex);
				} else {
					errx(1, "tryrdlock: %s",
					    strerror(e));
				}
			}
		}

		tspec.tv_sec = 0;
		tspec.tv_nsec = writer ? MSEC2NSEC(100) : MSEC2NSEC(1);
		nanosleep(&tspec, NULL);

		pthread_rwlock_unlock(&rwlock);
	}

	return (NULL);
}

int
main(int argc, char *argv[])
{
	pthread_t *threads;
	int numthreads;

	if (argc != 2)
		errx(1, "usage: %s num_threads", argv[0]);

	numthreads = atoi(argv[1]);
	if ((threads = malloc(sizeof (pthread_t) * numthreads)) == NULL)
		err(1, "malloc");

	for (int i = 0; i < numthreads; i++) {
		/*
		 * There is only one writer thread and it is created as the
		 * last thread so chances are the reader threads will be
		 * already running.
		 */
		if (pthread_create(&threads[i], NULL, thread,
		    i == numthreads - 1 ? (void *)true : (void *)false) != 0) {
			errx(1, "pthread create: %s", strerror(errno));
		}
	}

	for (int i = 0; i < numthreads; i++)
		pthread_join(threads[i], NULL);

	free(threads);

	printf("writer entered %zu times\n", wcount);
	printf("readers could not enter %zu times\n", rwaitcount);

	return (0);
}
