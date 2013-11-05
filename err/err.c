/*
 * Shows how to use err(3C) functions.
 */
#include <stdio.h>
#include <errno.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	errno = 0;

	/* Will not exit. Uses errno. */
	warn("warning %s", "GRR");

	/* Will not exit. Does not use errno. */
	warnx("warning without errno %s", "GRR");

	if (argc > 1) {
		errno = 13;
		err(4, "error exit %s", "GRR");
		/* Never reached. */
	} else {
		/* errx() does not use errno. */
		errx(3, "error exit without errno %s", "GRR");
		/* Never reached. */
	}

	return (0);
}
