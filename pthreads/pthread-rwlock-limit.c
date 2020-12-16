/*
 * Find out the lock count limit on your system.
 */

#define	_XOPEN_SOURCE   700

#include <err.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int
main(int argc, char *argv[])
{
	size_t numlocks;
	int e;

	if (argc != 2)
		errx(1, "usage: %s num_locks", argv[0]);

	numlocks = strtoll(argv[1], NULL, 10);
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
