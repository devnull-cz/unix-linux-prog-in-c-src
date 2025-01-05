/*
 * Simple demonstration of perhaps typical use case of R/W locks for ensuring
 * single writer or multiple readers semantics for a set of properties.
 * The prop_* functions would normally reside in a library.
 * Very loosely inspired by the libprop library on NetBSD.
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
#include <assert.h>
#include <sys/types.h>

#define	MSEC2NSEC(n)	((n) * 1000 * 1000)	// milli to nano

// Protects the 'properties' array below.
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

#define	MAX_PROPS	64

void *properties[MAX_PROPS];

#define	MAX_ITER	100

static void
millisleep(int msec)
{
	struct timespec tspec = { .tv_sec = 0, .tv_nsec = MSEC2NSEC(msec) };

	nanosleep(&tspec, NULL);
}

void
prop_set(size_t idx, void *val)
{
	int e;
	if ((e = pthread_rwlock_wrlock(&rwlock)) != 0)
		errx(1, "tryrdlock: %s", strerror(e));

	printf("writer %ld locked, setting %zu to %p\n",
	    pthread_self(), idx, val);

	properties[idx] = val;

	// Pretend there is some work involved.
	millisleep(500);

	pthread_rwlock_unlock(&rwlock);
}

void *
prop_get(size_t idx)
{
	// Simulate wild scheduling variance.
	millisleep(random() % 1000);

	int e;
	if ((e = pthread_rwlock_rdlock(&rwlock)) != 0)
		errx(1, "tryrdlock: %s", strerror(e));

	printf("reader %ld locked, getting %zu\n", pthread_self(), idx);

	/*
	 * Now, the 'properties' array should probably store an opaque type
	 * that would wrap the void * and also a reference count. The prop_*()
	 * functions might then bump the refcount.
	 */
	void *propval = properties[idx];

	pthread_rwlock_unlock(&rwlock);

	return (propval);
}

static void *
writer_thread(void *arg)
{
	int rnd = random() % MAX_PROPS;
	prop_set(rnd, arg);
	return (NULL);
}

static void *
reader_thread(void *arg)
{
	for (int i = 0; i < MAX_ITER; i++) {
		int rnd = random() % MAX_PROPS;
		void *val = prop_get(rnd);
		printf("got %p for %d\n", val, rnd);
	}
	return (NULL);
}

int
main(void)
{
	int numthreads = 32, i;
	pthread_t threads[numthreads];
	// Allocate the "data" on heap not to give bad example of setting
	// global properties' values with stack pointers.
	void **vals = malloc(numthreads * sizeof (void *));
	assert(vals != NULL);

	srandom(time(NULL));

	// Interleave thread creation so that each type has a chance to start.
	// Assumes each thread does some non-trivial work or sleeps for a bit.
	for (i = 0; i < numthreads; i++) {
		bool reader = i % 4 != 0;
		pthread_create(&threads[i], NULL,
		    reader ? reader_thread : writer_thread,
		    reader ? NULL : vals + i);
	}

	for (i = 0; i < numthreads; i++)
		pthread_join(threads[i], NULL);

	free(vals);
}
