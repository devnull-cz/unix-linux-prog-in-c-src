/*
 * The UNIX spec does not define what happens if synchronnous signals (SIGSEGV,
 * SIGILL, SIGBUS, and SIGFPE) are masked or ignored.  Most of the systems just
 * kill the process no matter how the signal mask is set.  However, you should
 * always be able to catch such a signal in order to print a user friendly
 * message, for example.  Note that after that, you better exit, never move on,
 * since what happens after returning from the handler is also undefined by the
 * standard.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define	MESSAGE "Signal caught !\n"

void
handle_segfault(int sig)
{
	write(1, MESSAGE, strlen(MESSAGE));
}

int
main(void)
{
	struct sigaction act = { 0 };

	act.sa_handler = handle_segfault;
	sigaction(SIGSEGV, &act, NULL);

	/*
	 * Intentionally in the loop.  Let's see what happens if we catch the
	 * signal but will not exit.
	 */
	while (1) {
		int *p = NULL;

		sleep(1);
		/* The following line definitely generates a signal. */
		*p = 1;
	}

	return (0);
}
