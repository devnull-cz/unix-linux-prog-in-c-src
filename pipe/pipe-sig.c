/*
 * Run like this and then press ^C
 *
 * for i in 1 2 3 4 5 6 7 8 9 10; do echo $i; sleep 1; done | \
 *    ./a.out | ./a.out | ./a.out | ./a.out
 *
 * Note: We could have used $(jot 10) for generating the sequence however it is
 *       not available on all systems except FreeBSD/Mac OS X.  You can use
 *       seq(1) though which tends to be present across most systems.
 */

#define _XOPEN_SOURCE 700	// needed for sigaction

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

void
handler(int yyy)
{
	(void) write(2, "Signal caught.\n", 14);
}

int
main(void)
{
	char c;
	int n;
	struct sigaction act;

	(void) sigemptyset(&act.sa_mask);
	act.sa_handler = handler;
	act.sa_flags = 0;
	(void) sigaction(SIGINT, &act, NULL);
	(void) fprintf(stderr, "%d: starting\n", getpid());

	while ((n = read(0, &c, 1)) != -1) {
		if (n == 0) {
			(void) fprintf(stderr, "%d: end of input\n", getpid());
			exit(0);
		}
		(void) write(1, &c, 1);
	}

	(void) fprintf(stderr, "%d: error %s\n", getpid(), strerror(errno));

	return (1);
}
