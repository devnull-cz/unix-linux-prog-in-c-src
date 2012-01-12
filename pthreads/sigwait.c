/*
 * An example on how to correctly handle signals in a threaded environment. Do
 * not use sigprocmask(3) as noted in the lecture materials (sigprocmask is
 * not defined for threaded environments by the specification).
 *
 * Compile with "-D_POSIX_PTHREAD_SEMANTICS" on Solaris, see sigwait(2) as to
 * why.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define	CYCLES	100

sigset_t sset;

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
	pthread_t t;
	int sig, n1 = 1, n2 = 2;

	/*
	 * We should not do this, we should never mask synchronnous signals like
	 * SIGSEGV, SIGILL, etc. See the slide on sigwait() in the lecture
	 * material to understand why. Also, you can see another source code
	 * file (sigwait-with-sync-signals.c). However, this one is just a
	 * simple example.
	 */
	sigfillset(&sset);
	pthread_sigmask(SIG_SETMASK, &sset, NULL);

	pthread_create(&t, NULL, thread, &n1);
	pthread_create(&t, NULL, thread, &n2);

	printf("You can ^C or ^\\ me now. Use 'kill -9 %d' " \
	    "from separate terminal to get rid of me.\n", getpid());
	while (1) {
		sigwait(&sset, &sig);
		printf("---> caught signal #%d\n", sig);
	}

	return (0);
}
