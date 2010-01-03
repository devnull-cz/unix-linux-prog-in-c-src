#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int a, b;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread(void *x)
{
	int data = (int) x;
	int err;

	sleep(1);

	/*
	 * On Solaris/Linux, it won't return any error since the default
	 * mutex type is NORMAL while it's ERRORCHECK on FreeBSD.
	 */
	if ((err = pthread_mutex_unlock(&mutex)) != 0)
		fprintf(stderr, "%s\n", strerror(err));

	return NULL;
}

int main(void)
{
	pthread_t t;

	pthread_create(&t, NULL, thread, (void *) 1);
	pthread_mutex_lock(&mutex);
	pthread_join(t, NULL);

	return (0);
}
