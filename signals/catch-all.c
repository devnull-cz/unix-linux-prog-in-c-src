/*
 * Catch all signals.  Try with ^C, ^\, also try resizing your terminal (you
 * should get SIGWINCH), you can also try "pkill catch-all" from another
 * terminal.  Note that ^Z (SIGTSTP) is also caught.
 *
 * You can only get rid of this by killing it by SIGKILL from another terminal
 * (pkill -9 catch-all).
 */

#define	_XOPEN_SOURCE	700

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	int sig;
	sigset_t set;

	sigfillset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);

	while (1) {
		sigwait(&set, &sig);
		printf("%d: got '%d'\n", getpid(), sig);
	}
}
