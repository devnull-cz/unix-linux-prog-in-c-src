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

#define	SECS	10

/* Should we use asynchronous cancel or not. */
int asynch = 0;

void *
mythread(void *x)
{
	int i = 1;
	time_t t2, t = time(NULL);

	if (asynch == 1)
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	else
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	printf("For the next %d seconds, the thread should not be canceled.\n",
	    SECS);
	printf("...unless PTHREAD_CANCEL_ASYNCHRONOUS was used\n");

	/*
	 * We can't use sleep(), poll() or select() since those usually use
	 * function calls that are cancellation points. Note that if you use
	 * PTHREAD_CANCEL_ASYNCHRONOUS then the cancelation occurs right after
	 * poll() below in main() is finished.
	 *
	 * Don't try to print a dot for every second or anything else since
	 * those would be cancelations points...
	 */
	while (time(NULL) - t < SECS) {
		;
	}

	/*
	 * We will not probably be able to see this since before this is
	 * printed, we enter some cancellation point and get terminated.
	 */
	printf("3 seconds passed; we should be canceled now...\n");

	/*
	 * In case we are not terminated in printf(), we wait for a producer
	 * here so we will block. However, open() is a cancellation point so we
	 * should exit as soon we call open().
	 */
	open("fifo", O_RDONLY);

	/* This is here so that you see that we won't get here. */
	fprintf(stderr, "ERROR: open() finished - SHOULD NOT HAVE HAPPENED !!!\n");

	return (NULL);
}

int
main(int argc, char **argv)
{
	int e;
	void *ptr;
	pthread_t t;

	fprintf(stderr, "Deferred cancellation in use, use with \"-a\" to get "
	    "asynchronous cancellation.\n");
	if (argc > 1 && strcmp(argv[1], "-a") == 0)
		asynch = 1;

	pthread_create(&t, NULL, mythread, NULL);

	/* Let the thread call pthread_setcanceltype(). */
	poll(NULL, 0, 500);

	/* We will block here until we get out of the while loop above. */
	if ((e = pthread_cancel(t)) != 0)
		errx(1, "pthread_cancel: %s", strerror(e));

	printf("Main has just called pthread_cancel() on the threads.\n");

	/*
	 * The pointer for canceled thread is defined in PTHREAD_CANCELED and
	 * contains something that does not reference valid memory. For example,
	 * on FreeBSD it was ((void *) 1), on Solaris I saw (void *)-19.
	 */
	if ((e = pthread_join(t, &ptr)) != 0)
		errx(1, "pthread_join: %s", strerror(e));
	else
		printf("pthread_join()'s returned pointer: 0x%0X\n", ptr);

	return (0);
}
