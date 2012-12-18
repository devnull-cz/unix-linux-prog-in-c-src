/*
 * Observe the behavior of unlocking a mutex in a thread which does not own it.
 * Run with multiple types on multiple systems and see how it differs
 * (especially the DEFAULT type).
 *
 * For example, OpenSolaris sets default to normal, FreeBSD 7.2 to errcheck,
 * Linux 2.6.32 kernel to normal.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

/* Needed for Linux to include the necessary mutex types */
#define	__USE_UNIX98

#include <pthread.h>

int a, b;
pthread_mutex_t mutex, mutex2, mutex3;

void
usage()
{
	printf("Program messes up with mutexes.\n");
	printf("Usage: not-my-lock [-h|-e|-n|-r]\n");
	printf("-e\tset mutex type to ERRORCHECK\n");
	printf("-n\tset mutex type to NORMAL\n");
	printf("-r\tset mutex type to RECURSIVE\n");
	printf("-h\tthis usage\n");
	printf("With no option: mutex type is set to DEFAULT.\n");

	exit(1);
}

void *
thread(void *x)
{
	int e;

	sleep(1);
	fprintf(stderr, "Attempting to unlock a mutex owned by somebody "
	    "else...\n");

	/*
	 * On Solaris/Linux, it won't return any error since the default
	 * mutex type is NORMAL while it's ERRORCHECK on FreeBSD.
	 */
	if ((e = pthread_mutex_unlock(&mutex)) != 0)
		fprintf(stderr, "%s\n\n", strerror(e));
	else
		fprintf(stderr, "No error detected.\n\n");

	return (NULL);
}

int
main(int argc, char *argv[])
{
	pthread_t t;
	pthread_mutexattr_t attr;
	int e, opt;

	pthread_mutexattr_init(&attr);

	while ((opt = getopt(argc, argv, "renh")) != -1) {
		switch (opt) {
			case 'n':
				if (pthread_mutexattr_settype(&attr,
				    PTHREAD_MUTEX_NORMAL) != 0)
					err(1, "mutexattr_settype");
				break;
			case 'r':
				if (pthread_mutexattr_settype(&attr,
				    PTHREAD_MUTEX_RECURSIVE) != 0)
					err(1, "mutexattr_settype");
				break;
			case 'e':
				if (pthread_mutexattr_settype(&attr,
				    PTHREAD_MUTEX_ERRORCHECK) != 0)
					err(1, "mutexattr_settype");
				break;
			case 'h':
				usage();
				break;
			default:
				if (pthread_mutexattr_settype(&attr,
				    PTHREAD_MUTEX_DEFAULT) != 0)
					err(1, "mutexattr_settype");
				warnx("Mutex set to default type");
				warnx("Run with -h to see other possible " \
				    "types");
				break;
		}
	}

	if (pthread_mutex_init(&mutex, &attr) == -1)
		err(1, "mutex_init for mutex");
	if (pthread_mutex_init(&mutex2, &attr) == -1)
		err(1, "mutex_init for mutex2");
	if (pthread_mutex_init(&mutex3, &attr) == -1)
		err(1, "mutex_init for mutex3");

	pthread_create(&t, NULL, thread, (void *) 1);
	fprintf(stderr, "Locking a mutex (and sleep 1 second).\n");
	pthread_mutex_lock(&mutex);
	pthread_join(t, NULL);

	fprintf(stderr, "Unlocking an unlocked mutex2.\n");
	if ((e = pthread_mutex_unlock(&mutex2)) != 0)
		fprintf(stderr, "%s\n\n", strerror(e));
	else
		fprintf(stderr, "No error detected.\n\n");

	fprintf(stderr, "Locking mutex3.\n");
	pthread_mutex_lock(&mutex3);
	fprintf(stderr, "And locking it again (will we deadlock?).\n");
	if ((e = pthread_mutex_lock(&mutex3)) != 0)
		fprintf(stderr, "%s\n\n", strerror(e));
	else
		fprintf(stderr, "No error detected.\n\n");

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);

	return (0);
}
