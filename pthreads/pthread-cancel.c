/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Copyright 2009 Jan Pechanec, Vladimir Kotal.  All rights reserved.
 * Use is subject to license terms.
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
#include <libgen.h>

#define	NSECS	5

int cancel_type;

#define	SECS	10

/* Should we use asynchronous cancel or not. */
int asynch = 0;

void *
mythread(void *x)
{
	int i = 1;
	time_t t2, t = time(NULL);

<<<<<<< local
	/* Controlled by -a|-d switches, see main(). */
	pthread_setcanceltype(cancel_type, NULL);
=======
	if (asynch == 1)
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	else
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
>>>>>>> other

<<<<<<< local
	if (cancel_type == PTHREAD_CANCEL_DEFERRED)
		printf("For the next %d seconds, the thread should not be "
		    "cancelled (deferred cancellation was used).\n", NSECS);
	else
		printf("The thread will be cancelled right away (asynchronous "
		    "cancellation was used).\n");
=======
	printf("For the next %d seconds, the thread should not be canceled.\n",
	    SECS);
	printf("...unless PTHREAD_CANCEL_ASYNCHRONOUS was used\n");
>>>>>>> other

	/*
	 * We can't use sleep(), poll() or select() since those usually use
	 * function calls that are cancellation points. Note that if you use
	 * PTHREAD_CANCEL_ASYNCHRONOUS then the cancelation occurs right after
	 * poll() below in main() is finished.
	 *
	 * Don't try to print a dot for every second or anything else since
	 * those would be cancelations points...
	 */
<<<<<<< local
	while (time(NULL) - t < NSECS) {
=======
	while (time(NULL) - t < SECS) {
>>>>>>> other
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

<<<<<<< local
	if (argc != 2)
		errx(1, "usage: %s -a|-d", basename(argv[0]));
=======
	fprintf(stderr, "Deferred cancellation in use, use with \"-a\" to get "
	    "asynchronous cancellation.\n");
	if (argc > 1 && strcmp(argv[1], "-a") == 0)
		asynch = 1;
>>>>>>> other

<<<<<<< local
	if (strcmp(argv[1], "-a") == 0)
		cancel_type = PTHREAD_CANCEL_ASYNCHRONOUS;
	else if (strcmp(argv[1], "-d") == 0)
		cancel_type = PTHREAD_CANCEL_DEFERRED;
	else
		errx(1, "usage: %s -a|-d", basename(argv[0]));

=======
>>>>>>> other
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
