/*
 * Wanna see how SIGKILL can be caught?
 *
 * Take FreeBSD 5.3, some threads from libpthread (-lpthread, not -lthr nor
 * -lc_r), suspend the program with SIGTSTP (= Ctrl-Z) and then kill it with
 * SIGKILL from another terminal. The app will wake up with "---> caught signal
 * #9" message.
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700	// for sigset_t

#define	_POSIX_PTHREAD_SEMANTICS 1

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define	CYCLES  100

/*
 * Create two threads, mask all signals in their signal masks and let them
 * run. In the main thread, process all the signalling stuff.
 */
sigset_t mysigset;

void *
thread(void *x)
{
	int i;

	for (i = 0; i < CYCLES; ++i) {
		printf("thread %d (loop #%d)\n", *((int *) x), i);
		sleep(1);
	}

	return (NULL);
}

int
main(void)
{
	int sig;
	pthread_t t;
	int n1 = 1, n2 = 2;

	printf("my PID is %d...\n", getpid());

	sigfillset(&mysigset);
	pthread_sigmask(SIG_SETMASK, &mysigset, NULL);

	pthread_create(&t, NULL, thread, &n1);
	pthread_create(&t, NULL, thread, &n2);

	while (1) {
		sigwait(&mysigset, &sig);
		printf("---> caught signal #%d\n", sig);
	}

	return (0);
}
