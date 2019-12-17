/*
 * An example on how to correctly handle signals in a threaded environment.
 * Do not use sigprocmask(3) as noted in the lecture materials (sigprocmask
 * is not defined for threaded environments by the specification).
 *
 * Compile with "-D_POSIX_PTHREAD_SEMANTICS" on Solaris, see sigwait(2) as to
 * why.
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700

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

/*
 * We should never mask synchronnous signals like SIGSEGV.  It is undefined what
 * happens if such a signal is delivered when masked (module some specific
 * situations we dot care about here).
 */
int sync_sigs[] = { SIGFPE, SIGILL, SIGSEGV, SIGBUS };

int
main(void)
{
	pthread_t t1, t2;
	int sig, n1 = 1, n2 = 2;

	sigfillset(&sset);
	for (int i = 0; i < sizeof (sync_sigs) / sizeof (int); ++i)
		sigdelset(&sset, sync_sigs[i]);
	pthread_sigmask(SIG_SETMASK, &sset, NULL);

	pthread_create(&t1, NULL, thread, &n1);
	pthread_create(&t2, NULL, thread, &n2);

	printf("You can to ^C, ^\\, or ^Z me now.  Use 'kill -9 %d' " \
	    "from separate terminal to get rid of me.\n", getpid());
	while (1) {
		sigwait(&sset, &sig);
		printf("---> caught signal #%d\n", sig);
	}
}
