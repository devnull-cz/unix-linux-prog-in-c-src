/*
 * More granular sleep using poll(). See also nanosleep.c.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <poll.h>

#define	CYCLES	1000

int
main(void)
{
	int i;

	for (i = 0; i < CYCLES; ++i) {
		putc('.', stderr);
		/* wait 50 miliseconds between the dots */
		poll(NULL, 0, 50);

		/* make it nice */
		if ((i + 1) % 19 == 0)
			putc('\n', stderr);
	}
	
	return (0);
}
