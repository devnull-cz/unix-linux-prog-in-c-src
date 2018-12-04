/*
 * Create one thread and let it print a few numbers in a loop with
 * optionally waiting for it in the main thread.
 *
 * On Linux you can use 'ltrace -S ./pthread-create' to see the effect
 * of pthread_create().
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
main(int argc, char *argv[])
{
	pthread_t t;

	(void) pthread_create(&t, NULL, thread, NULL);

	/*
	 * We have to do this for now so that we do not exit from main before
	 * the thread has finished. Note that there is a better way (= correct
	 * way) of doing this - joining the thread.
	 */
	if (argc > 1)
		(void) sleep(5);

	return (0);
}
