#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>

void *thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		printf("thread loop #%d\n", i);
		sleep(1);
	}

	return (NULL);
}

int main(void)
{
	pthread_t t;

	pthread_create(&t, NULL, thread, NULL);
	/* for now... */
	sleep(5);

	return (0);
}
