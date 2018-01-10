/*
 * Create one thread and let it print a few numbers in a loop.
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *
thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		(void) printf("thread loop #%d\n", i);
		(void) sleep(1);
	}
	return (NULL);
}

int
main(void)
{
	pthread_t t;

	(void) pthread_create(&t, NULL, thread, NULL);
	/*
	 * We have to do this for now so that we do not exit from main before
	 * the thread has finished. Note that there is a better way (= correct
	 * way) of doing this.
	 */
//	(void) sleep(5);
	return (0);
}
