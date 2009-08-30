/*
 * This example program shows a correct use of the 4th parameter in
 * pthread_create(). Compare to wrong-use-of-arg.c.
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		printf("thread %d (loop #%d)\n", *((int *) x), i);
		sleep(1);
	}
	return (NULL);
}

int main(void)
{
	int i;
	int id[5];
	pthread_t t;

	for (i = 0; i < 5; ++i) {
		id[i] = i;
		pthread_create(&t, NULL, thread, id + i);
		/*
		pthread_yield();
		*/
	}

	/* avoiding pthread_join() for now */
	sleep(6);

	return (0);
}
