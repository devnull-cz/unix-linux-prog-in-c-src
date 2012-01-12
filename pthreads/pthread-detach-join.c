/*
 * An example what happens if pthread_join() is called on a detached thread.
 * Join should fail with EINVAL error which means in this context means that the
 * thread is detached and cannot be joined. See also set-detachstate.c.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <pthread.h>

void *
thread(void *x)
{
	int i;
	void *p;

	/*
	 * This "guarantees" that when entering pthread_join() the thread is
	 * still joinable. However, pthread_join() returns an error as soon as
	 * the thread is made detachable.
	 */
	sleep(1);
	pthread_detach(pthread_self());

	for (i = 0; i < 5; ++i) {
		printf("thread %d (loop #%d)\n", *((int *) x), i);
		sleep(1);
	}
	
	return ((void *)x);
}

int
main(void)
{
	int e;
	void *p1;
	pthread_t t1;
	int n1 = 1;

	pthread_create(&t1, NULL, thread, &n1);

	printf("thread created\n");
	printf("main thread starts waiting for thread completion...\n");

	if ((e = pthread_join(t1, &p1)) != 0)
		errx(1, "pthread_join: %s", strerror(e));

	printf("thread %d returned %d\n", n1, *((int *)p1));

	return (0);
}
