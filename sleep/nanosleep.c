/*
 * More granular sleep nanosleep(). Also, see poll-sleep.c.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <poll.h>
#include <time.h>

#define	CYCLES	1000

int
main(void)
{
	int i;
	struct timespec ts;

	ts.tv_sec = 0;
	ts.tv_nsec = 50000000;

	for (i = 0; i < CYCLES; ++i) {
		putc('.', stderr);
		/* wait 50 miliseconds between the dots */
		nanosleep(&ts, NULL);

		/* make it nice */
		if ((i + 1) % 19 == 0)
			putc('\n', stderr);
	}

	return (0);
}
