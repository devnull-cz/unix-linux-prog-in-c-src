/*
 * Compile with and without -DPOSIX_SIGNALS and try to run like this
 * on {FreeBSD,Linux}/Solaris to see the difference in behavior:
 *   0. compile 2 programs
 *      gcc -o sig1 signal-vs-sigaction.c
 *      gcc -DPOSIX_SIGNALS -o sig2 signal-vs-sigaction.c
 *   1. run the first program
 *      ./sig1 &
 *   2. send the 2 signals in quick succession (in the 10 seconds window)
 *      kill -INT `pgrep sig1`
 *      kill -INT `pgrep sig1`
 *   3. run the second program
 *      ./sig2 &
 *   4. send the 2 signals in quick succession (in the 10 seconds window)
 *      kill -INT `pgrep sig2`
 *      kill -INT `pgrep sig2`
 *   5. observe the behavior (check if the programs are still around)
 *
 *  vlada@devnull.cz, 2009
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void
sig_handler(int s)
{
	printf("Got signal! Sleeping.\n");

	sleep(10);

	printf("returning from signal handler\n");
}

int
main(void)
{
	struct sigaction s_action;

	/*
	 * You may experiment with other signals but note that some shell might
	 * automatically block some signals; bash(1) sometimes block SIGHUP in
	 * the new process, for example.
	 */
	printf("Setting signal handler: ");
#ifdef POSIX_SIGNALS
	printf("sigaction\n");
	(void) sigemptyset(&s_action.sa_mask);
	s_action.sa_handler = sig_handler;
	s_action.sa_flags = 0;
	(void) sigaction(SIGINT, &s_action, (struct sigaction *)NULL);
#else
	printf("signal\n");
	signal(SIGINT, sig_handler);
#endif

	printf("Waiting for signal\n");
	while (1)
		pause();

	return (0);
}
