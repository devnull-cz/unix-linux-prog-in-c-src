/*
 * An example that we should never block synchronous signals since those cannot
 * be handled via the dedicated signal manager thread. Those go directly to the
 * thread that caused the exception and if such signal is blocked in that
 * thread, it *may* be never received, based on the platform you will use. Not
 * delivering such signals can cause serious problems - you might continue to
 * run with corrupted memory, for example, without knowing that.
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
#include <libgen.h>
#include <err.h>
#include <pthread.h>

#define	CYCLES	100

sigset_t sset;
int k_opt = 0;

void *
thread(void *x)
{
	int *p = NULL;

	if (k_opt == 1)
		pthread_kill(pthread_self(), SIGSEGV);
	else
		/* Simulating memory corruption. */
		*p = 0;

	return (NULL);
}

int
main(int argc, char **argv)
{
	pthread_t t;
	int opt, sig, n1 = 1, n2 = 2;

	while ((opt = getopt(argc, argv, "k")) != -1) {
		switch (opt) {
		case 'k':
			printf(">>> Will use pthread_kill() to send "
			    "SIGSEGV.<<<\n\n");
			k_opt = 1;
			break;
		case '?':
			errx(1, "Usage: %s [-k]\n", basename(argv[0]));
			break;
		}
	}

	if (k_opt == 0) {
		printf(">>> SIGSEGV will be generated by corrupting the "
		    "heap.<<<\n");
		printf("Use '-k' option to use pthread_kill()\n\n");
	}

	printf("While all the threads mask SIGSEGV, process might be killed "
	    "based on the system.\nNo matter what, pthread_kill() will "
	    "probably NOT kill the process.\n");
	printf("However, memory curruption probably will.\n");
	printf("Whatever is the outcome, you should never block synchronous "
	    "signals.\n\n");

	/*
	 * We intentionally mask all signals including the synchronous ones
	 * (SIGSEGV, SIGILL, SIGBUS, SIGFPE).
	 */
	sigfillset(&sset);
	pthread_sigmask(SIG_SETMASK, &sset, NULL);

	pthread_create(&t, NULL, thread, &n1);
	pthread_create(&t, NULL, thread, &n2);

	printf("You can ^C or ^\\ me now. Use 'kill -9 %d' from separate "
	    "terminal to get rid of me.\n", getpid());
	while (1) {
		sigwait(&sset, &sig);
		printf("---> caught signal #%d\n", sig);
	}

	return (0);
}
