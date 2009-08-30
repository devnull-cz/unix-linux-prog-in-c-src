/*
 * A short excercise with cancelling a thread. See comments for more
 * information. Also, use PTHREAD_CANCEL_ASYNCHRONOUS to see the difference.
 *
 * (c) jp@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <strings.h>
#include <err.h>
#include <poll.h>
#include <time.h>

void *
thread(void *x)
{
	int i = 1;
	time_t t2, t = time(NULL);

	/* pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); */
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	printf("for the next 3 seconds, the thread should not be canceled\n");
	printf("...unless PTHREAD_CANCEL_ASYNCHRONOUS was used\n");

	/*
	 * We can't use sleep(), poll() or select() since those usually use
	 * function calls that are cancellation points. Note that if you use
	 * PTHREAD_CANCEL_ASYNCHRONOUS then the cancelation occurs right after
	 * poll() below is finished.
	 *
	 * Don't try to print a dot for every second or anything else since
	 * those would be cancelations points...
	 */
	while (time(NULL) - t < 3) {
		;
	}

	printf("3 seconds passed; we should be canceled now...\n");

	/*
	 * Note that we wait for a producer here so we will block. However,
	 * open() is a cancellation point so we should exit as soon we call
	 * open().
	 */
	open("fifo", O_RDONLY);

	/* this is here so that you see that we won't get here */
	fprintf(stderr, "open() finished\n");

	return (NULL);
}

int
main(void)
{
	int e;
	void *ptr;
	pthread_t t;

	pthread_create(&t, NULL, thread, NULL);

	/* let the thread call pthread_setcanceltype() */
	poll(NULL, 0, 100);

	/* we will block here until we get out of the while loop above */
	if ((e = pthread_cancel(t)) != 0)
		errx(1, "pthread_cancel: %s", strerror(e));

	/*
	 * The pointer for canceled thread is defined in PTHREAD_CANCELED and
	 * contains something that does not reference a valid memory. For
	 * example, on FreeBSD it's ((void *) 1), on Solaris it's (void *)-19.
	 */
	if ((e = pthread_join(t, &ptr)) != 0)
		errx(1, "pthread_join: %s", strerror(e));
	else
		printf("pthread_join value: %x\n", ptr);

	return (0);
}
