/*
 * run like this and then press ^C
 *
 * for i in 1 2 3 4 5 6 7 8 9 10; do echo $i; sleep 1; done | \
 *    ./a.out | ./a.out | ./a.out | ./a.out
 *
 * Side note: We could have used `jot 10` for generating the sequence however
 * 	      it is not available on all systems except FreeBSD/Mac OS X.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

void
handler(int yyy)
{
	write(2, "signal caught\n", 14);
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
	fprintf(stderr, "%d: starting\n", getpid());

	while ((n = read(0, &c, 1)) != -1) {
		if (n == 0) {
			fprintf(stderr, "%d: end of input\n", getpid());
			exit(0);
		}
		write(1, &c, 1);
	}

	fprintf(stderr, "%d: error %s\n", getpid(), strerror(errno));

	return (1);
}
