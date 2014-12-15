/*
 * Observe the behavior of unlocking a mutex in a thread which does not own it.
 * Run with multiple types on multiple systems and see how it differs
 * (especially the DEFAULT type).
 *
 * For example, Solaris 11 sets default to normal, FreeBSD 7.2 to errcheck,
 * Linux 2.6.32 kernel to normal.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <assert.h>

/* Needed for Linux to include the necessary mutex types */
#define	__USE_UNIX98

#include <pthread.h>

int a, b;
pthread_mutex_t mutex, mutex2, mutex3;

void
usage()
{
	(void) printf("Program messes up with mutexes.\n");
	(void) printf("Usage: not-my-lock [-h|-e|-n|-r]\n");
	(void) printf("-e\tset mutex type to ERRORCHECK\n");
	(void) printf("-n\tset mutex type to NORMAL\n");
	(void) printf("-r\tset mutex type to RECURSIVE\n");
	(void) printf("-h\tthis usage\n");
	(void) printf("With no option: mutex type is set to DEFAULT.\n");
	exit(1);
}

void *
thread(void *x)
{
	int e;

	sleep(1); /* Give the main thread some time to lock the mutex. */
	(void) fprintf(stderr, "Attempting to unlock a mutex owned by somebody "
	    "else...\n");

	/*
	 * On Solaris/Linux, it won't return any error since the default
	 * mutex type is NORMAL while it's ERRORCHECK on FreeBSD.
	 */
	if ((e = pthread_mutex_unlock(&mutex)) != 0)
		(void) fprintf(stderr, "%s\n\n", strerror(e));
	else
		(void) fprintf(stderr, "No error detected.\n\n");

	return (NULL);
}

int
main(int argc, char *argv[])
{
	pthread_t t;
	pthread_mutexattr_t attr;
	int e, opt, ret;

	(void) pthread_mutexattr_init(&attr);

	while ((opt = getopt(argc, argv, "renh")) != -1) {
		switch (opt) {
		case 'n':
			if (pthread_mutexattr_settype(&attr,
			    PTHREAD_MUTEX_NORMAL) != 0) {
				err(1, "mutexattr_settype");
			}
			break;
		case 'r':
			if (pthread_mutexattr_settype(&attr,
			    PTHREAD_MUTEX_RECURSIVE) != 0) {
				err(1, "mutexattr_settype");
			}
			break;
		case 'e':
			if (pthread_mutexattr_settype(&attr,
			    PTHREAD_MUTEX_ERRORCHECK) != 0) {
				err(1, "mutexattr_settype");
			}
			break;
		case 'h':
		default:
			usage();
			break;
		}
	}

	if (argc == 1) {
		ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_DEFAULT);
		if (ret != 0)
			err(1, "mutexattr_settype");
		warnx("Mutex set to default type");
		warnx("Run with -h to see other possible types\n");
	}

	if (pthread_mutex_init(&mutex, &attr) == -1)
		err(1, "mutex_init for mutex");
	if (pthread_mutex_init(&mutex2, &attr) == -1)
		err(1, "mutex_init for mutex2");
	if (pthread_mutex_init(&mutex3, &attr) == -1)
		err(1, "mutex_init for mutex3");

	ret = pthread_create(&t, NULL, thread, (void *)1);
	assert(ret == 0);
	(void) fprintf(stderr, "Locking a mutex (and sleep 1 second).\n");
	(void) pthread_mutex_lock(&mutex);
	pthread_join(t, NULL);

	(void) fprintf(stderr, "Unlocking an unlocked mutex2.\n");
	if ((e = pthread_mutex_unlock(&mutex2)) != 0)
		(void) fprintf(stderr, "%s\n\n", strerror(e));
	else
		(void) fprintf(stderr, "No error detected.\n\n");

	(void) fprintf(stderr, "Locking mutex3.\n");
	(void) pthread_mutex_lock(&mutex3);
	(void) fprintf(stderr, "And locking it again (will we deadlock?).\n");
	if ((e = pthread_mutex_lock(&mutex3)) != 0)
		(void) fprintf(stderr, "%s\n\n", strerror(e));
	else
		(void) fprintf(stderr, "No error detected.\n\n");

	(void) pthread_mutex_destroy(&mutex);
	(void) pthread_mutex_destroy(&mutex2);
	(void) pthread_mutex_destroy(&mutex3);

	return (0);
}
