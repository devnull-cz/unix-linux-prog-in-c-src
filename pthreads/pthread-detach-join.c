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
	int i, e;

	/*
	 * Detaching from within the thread function means that the thread is
	 * still joinable at this point. However, pthread_join() should return
	 * an error as soon as the thread is made detachable.
	 */
	printf("Thread started.\n");
	if ((e = pthread_detach(pthread_self())) != 0)
		errx(1, "pthread_detach: %s", strerror(e));
	printf("Thread detached.\n");

	for (i = 0; i < 5; ++i) {
		printf("Thread %d (loop #%d).\n", *((int *) x), i);
		sleep(1);
	}

	return ((void *)x);
}

int
main(void)
{
	void *p1;
	pthread_t t1;
	int e, n1 = 1;

	pthread_create(&t1, NULL, thread, &n1);
	printf("Thread created.\n");

	/* Give the thread some cycles to detach and run. */
	sleep(3);

	printf("Main thread starts waiting for thread completion...\n");
	if ((e = pthread_join(t1, &p1)) != 0)
		errx(1, "pthread_join: %s", strerror(e));

	printf("Thread %d returned %d.\n", n1, *((int *)p1));

	return (0);
}
