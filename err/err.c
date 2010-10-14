/*
 * Shows how to use err(3C) functions.
 */
#include <errno.h>
#include <err.h>

int
main(void)
{
	errno = 13;
	err(3, "grr %s", "GRR");
	/* Never reached. */

	/* errx() does not use errno. */
	errx(3, "grr %s", "GRR");

	/* Will not exit. Uses errno. */
	warn("grr %s", "GRR");

	/* Will not exit. Does not use errno. */
	warnx("grr %s", "GRR");

	printf("after err()\n");
	return (0);
}
