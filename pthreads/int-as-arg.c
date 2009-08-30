/*
 * If you need to pass just a thread ID only, you can use the void pointer
 * itself. However, this way is defined as not portable in the specification
 * so be careful. Compare to wrong-use-of-arg.c and correct-use-of-arg.c.
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

void *thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		printf("thread %d (loop #%d)\n", (int) x, i);
		sleep(1);
	}
	return (NULL);
}

int main(void)
{
	pthread_t t;
	int i;

	/* let's not shoot our leg off */
	assert(sizeof(int) <= sizeof(void *));

	for (i = 0; i < 5; ++i) {
		pthread_create(&t, NULL, thread, (void *) i);
	}

	/* avoid pthread_join() for now */
	sleep(6);

	return (0);
}
