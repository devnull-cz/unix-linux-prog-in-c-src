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
