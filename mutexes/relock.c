#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int n;
	pthread_mutexattr_t attr;
	pthread_mutex_t mutex;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&mutex, &attr);

	pthread_mutex_lock(&mutex);
	if ((n = pthread_mutex_lock(&mutex)) != 0) {
		printf("pthread_mutex_lock(): %s\n", strerror(n));
	}

	return (0);
}
