/*
 * Very simple cat(1)-like program. Example program on open/read/write/close
 * system calls.
 *
 * (c) jp@devnull.cz
 */

#include <unistd.h>
#include <fcntl.h>
#include <err.h>

/* Needed for basename(3C) on Solaris. */
#include <libgen.h>

/* cat(1) returns 0 if all files were processed, non-zero otherwise. */
int ret;

void
do_cat(char *file)
{
	int fd, n;
	char buf[1024];

	if ((fd = open(file, O_RDONLY)) == -1) {
		warn("open");
		ret = 1;
		return;
	}

	while ((n = read(fd, buf, 1024)) > 0) {
		write(1, buf, n);
	}

	/* Special case. */
	if (n == -1) {
		warn("read");
		ret = 1;
	}

	/*
	 * Never forget, this could lead to a situation similar to having memory
	 * leaks. Could be a problem for long running applications or daemons.
	 */
	close(fd);
}

int
main(int argc, char **argv)
{
	int i = 1;

	if (argc == 1)
		errx(1, "usage: %s <file> [<file2> ...]", basename(argv[0]));

	while (argv[i] != NULL) {
		do_cat(argv[i]);
		++i;
	}

	return (0);
}
