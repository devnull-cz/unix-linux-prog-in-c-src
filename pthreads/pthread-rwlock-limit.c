/*
 * Find out the lock count limit on your system.
 */

#define	_XOPEN_SOURCE   700

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int
main(int argc, char *argv[])
{
	size_t numlocks;

	if (argc != 2)
		errx(1, "usage: %s num_locks", argv[0]);

	numlocks = strtoll(argv[1], NULL, 10);
	printf("will try: %zu\n", numlocks);

	for (size_t i = 0; i < numlocks; i++) {
		if (pthread_rwlock_rdlock(&rwlock) != 0) {
			if (errno == EAGAIN) {
				printf("lock count = %zu\n", i);
				exit(0);
			} else {
				errx(1, "rdlock: %s", strerror(errno));
			}
		}
	}

	return (0);
}
