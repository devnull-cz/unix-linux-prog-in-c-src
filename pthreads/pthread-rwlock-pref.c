/*
 * A naive test (i.e. not considering complexities like length of a waiting
 * thread queue) to see if a RW lock implementation is writer biased.
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
#include <sys/types.h>

#define	MSEC2NSEC(n)	((n) * 1000 * 1000)	// milli to nano

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

static void
millisleep(int msec)
{
	struct timespec tspec = { .tv_sec = 0, .tv_nsec = MSEC2NSEC(msec) };

	nanosleep(&tspec, NULL);
}


void *
writer_thread(void *arg)
{
	printf("writer %ld trying to get the lock\n", (intptr_t)arg);

	pthread_rwlock_wrlock(&rwlock);

	printf("writer %ld locked\n", (intptr_t)arg);

	millisleep(5000);

	pthread_rwlock_unlock(&rwlock);
	printf("writer %ld unlocked\n", (intptr_t)arg);

	return (NULL);
}

void *
reader_thread(void *arg)
{
	int e;

	printf("reader trying to get the lock\n");
	if ((e = pthread_rwlock_rdlock(&rwlock)) != 0)
		errx(1, "tryrdlock: %s", strerror(e));

	printf("reader locked\n");
	millisleep(500);

	pthread_rwlock_unlock(&rwlock);

	return (NULL);
}

int
main(void)
{
	int numthreads = 3, i = 0;
	pthread_t threads[numthreads];

	/* We should really not cast an int like this... */
	pthread_create(&threads[i++], NULL, writer_thread, (void *)1);
	millisleep(500); // give the writer chance to run

	pthread_create(&threads[i++], NULL, reader_thread, NULL);
	millisleep(100); // let reader start

	/*
	 * Before starting this thread, we have a writer holding a lock while a
	 * reader is waiting.
	 * */
	pthread_create(&threads[i++], NULL, writer_thread, (void *)2);

	for (i = 0; i < numthreads; i++)
		pthread_join(threads[i], NULL);
}
