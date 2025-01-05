/*
 * Find out the lock count limit on your system, assuming it is less than
 * ULONG_MAX.
 */

#define	_XOPEN_SOURCE   700

#include <err.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int
main(int argc, char *argv[])
{
	size_t numlocks = ULONG_MAX;
	int e;

	printf("will try: %zu\n", numlocks);

	for (size_t i = 0; i < numlocks; i++) {
		if ((e = pthread_rwlock_rdlock(&rwlock)) != 0) {
			if (e == EAGAIN) {
				printf("maximum lock count = %zu\n", i);
				exit(0);
			}
			errx(1, "rdlock: %s", strerror(e));
		}
	}
	printf("succeeded\n");
}
