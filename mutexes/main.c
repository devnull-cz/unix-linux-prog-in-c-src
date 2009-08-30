#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int a, b;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread(void *x)
{
	int data = (int) x;

	while (1) {

		/* compile w/ and w/o lock/unlock to see the difference */
		pthread_mutex_lock(&mutex);
		if (a != b) {
			if (data == 0)
				write(1, "|", 1);
			else
				write(1, "_", 1);
		}

		a = data;
		//
		b = data;
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main(void)
{
	pthread_t t1, t2;

	pthread_create(&t1, NULL, thread, (void *) 0);
	pthread_create(&t2, NULL, thread, (void *) 1);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return (0);
}
