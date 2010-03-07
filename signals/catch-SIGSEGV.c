/*
 * The specification does not define what happens if synchronnous signals
 * (SIGSEGV, SIGILL, SIGBUS, and SIGFPE) are blocked or ignored. Most of the
 * systems just kill the process no matter how the signal mask is set. However,
 * you should always be able to catch such signal in order to print a user
 * friendly message, for example. Note that after that, you should always exit,
 * never move on, since what would happen then is also undefined by the spec.
 *
 * (c) jp@devnull.cz
 */
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define MESSAGE "Signal caught !\n"

void
ctrl_c(int sig)
{
	write(1, MESSAGE, strlen(MESSAGE));
}

int
main(void)
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = ctrl_c;
	sigaction(SIGSEGV, &act, NULL);

	/*
	 * Intentionally in the loop. Let's see what happens if we catch the
	 * signal but will not exit;
	 */
	while (1) {
		int *p = NULL;

		sleep(1);
		/* The following line definitely generates the SIGSEGV signal. */
		*p = 1;
	}

	return (0);
}
