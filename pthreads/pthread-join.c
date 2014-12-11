/*
 * An example on pthread_join() usage. See that we get the return value from
 * each thread in respective pthread_join() calls.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *
thread(void *x)
{
	int i;

	for (i = 0; i < 5; ++i) {
		printf("thread %d (loop #%d)\n", *((int *) x), i);
		sleep(1);
	}

	return ((void *)x);
}

int
main(void)
{
	void *p1, *p2;
	pthread_t t1, t2;
	int n1 = 1, n2 = 2;

	pthread_create(&t1, NULL, thread, &n1);
	pthread_create(&t2, NULL, thread, &n2);

	printf("threads created\n");
	printf("main thread starts waiting for threads completion...\n");

	pthread_join(t1, &p1);
	pthread_join(t2, &p2);

	printf("thread %d returned %d\n", n1, *((int *)p1));
	printf("thread %d returned %d\n", n2, *((int *)p2));

	return (0);
}
