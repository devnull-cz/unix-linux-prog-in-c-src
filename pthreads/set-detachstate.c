/*
 * Another way how to create detached threads. See also pthread-detach-join.c.
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <err.h>

void
*thread(void *x)
{
	int i;

	if (*((int *)x) == 1)
		poll(NULL, 0, 500);

	for (i = 0; i < 10; ++i) {
		printf("thread %d (loop #%d)\n", *((int *) x), i);
		sleep(1);
	}

	return (NULL);
}

int
main(void)
{
	int e;
	pthread_t t, t2;
	pthread_attr_t a;
	int n1 = 1, n2 = 2;

	pthread_attr_init(&a);
	pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

	pthread_create(&t, &a, thread, &n1);
	pthread_create(&t2, &a, thread, &n2);
	printf("threads created, will try to join them now\n");

	/* wait half a second for the threads to start */
	(void) poll(NULL, 0, 500);

	if ((e = pthread_join(t, NULL)) != 0)
		errx(1, "pthread_join: %s\n", strerror(e));

	if ((e = pthread_join(t2, NULL)) != 0)
		errx(1, "pthread_join: %s\n", strerror(e));

	return (0);
}
