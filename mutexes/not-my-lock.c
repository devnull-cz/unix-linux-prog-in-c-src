/*
 * Observe the behavior of unlocking a mutex in a thread which does not own it.
 * Run with multiple types on multiple systems and see how it differs.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Needed for Linux to include the necessary mutex types */
#define	__USE_UNIX98

#include <pthread.h>

int a, b;
pthread_mutex_t mutex;

void usage()
{
	printf("usage: not-my-lock [-h|-e|-n|-r]\n");
	printf("-e\tset mutex type to ERRORCHECK\n");
	printf("-n\tet mutex type to NORMAL\n");
	printf("-r\tet mutex type to RECURSIVE\n");
	printf("-h\tthis usage\n");
	printf("default: mutex type is set to DEFAULT\n");
	printf("\n");

	exit(1);
}

void *thread(void *x)
{
	int data = (int) x;
	int err;

	sleep(1);
	fprintf(stderr, "Attempting unlock\n");

	/*
	 * On Solaris/Linux, it won't return any error since the default
	 * mutex type is NORMAL while it's ERRORCHECK on FreeBSD.
	 */
	if ((err = pthread_mutex_unlock(&mutex)) != 0)
		fprintf(stderr, "%s\n", strerror(err));

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t t;
	pthread_mutexattr_t attr;
	int opt;
	
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
		err(1, "mutex_init");
	pthread_create(&t, NULL, thread, (void *) 1);
	fprintf(stderr, "Locking\n");
	pthread_mutex_lock(&mutex);
	pthread_join(t, NULL);
	pthread_mutex_destroy(&mutex);

	return (0);
}
