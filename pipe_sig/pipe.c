/*
 * run like this and then press ^C
 *
 * We could have used `jot 10` for generating the sequence
 * however it is not available on all systems except FreeBSD/Mac OS X.
 *
 * for i in 1 2 3 4 5 6 7 8 9 10; do echo $i; sleep 1; done | \
 *    ./a.out | ./a.out | ./a.out | ./a.out
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void *
xxx(int yyy)
{
	fprintf(stderr, "%d: signal caught\n", getpid());
	return (NULL);
}

int
main(void)
{
	char c;
	int n;

	/* XXX do not use signal() ! */
	signal(2, (sig_t) xxx);
	fprintf(stderr, "%d: starting\n", getpid());

	while ((n = read(0, &c, 1)) != -1) {
		if (n == 0) {
			fprintf(stderr, "%d: end of input\n", getpid());
			exit(0);
		}
		write(1, &c, 1);
	}

	fprintf(stderr, "%d: error %d\n", getpid(), errno);

	return (1);
}
