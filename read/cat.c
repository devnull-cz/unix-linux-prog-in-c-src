/*
 * Very simple cat(1)-like program. Example program on open/read/write/close
 * system calls.
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700	// for getopt()

#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/* Needed for basename(3C) on Solaris. */
#include <libgen.h>

/* cat(1) returns 0 if all files were processed, non-zero otherwise. */
int ret;

void
do_cat(char *file, int bufsize)
{
	int fd, n;
	char *buf = NULL;

	/* Bail out right away, we cannot proceed with no memory left. */
	if ((buf = malloc(bufsize)) == NULL)
		err(1, "malloc");

	if ((fd = open(file, O_RDONLY)) == -1) {
		warn("open");
		ret = 1;
		free(buf);
		return;
	}

	while ((n = read(fd, buf, bufsize)) > 0) {
		/* Yes, we assume writing to tty will not fail. */
		write(1, buf, n);
	}

	/* Special case. */
	if (n == -1) {
		warn("read");
		ret = 1;
	}

	/*
	 * Never forget, this could lead to a situation similar to having memory
	 * leaks.  Could be a problem for long running applications or daemons.
	 */
	close(fd);
	free(buf);
}

void
usage(char *argv0)
{
	errx(1, "usage: %s [-b bufsize] <file> [<file2> ...]", basename(argv0));
}

int
main(int argc, char **argv)
{
	int opt, bufsize = 4096;

	if (argc == 1)
		usage(argv[0]);

	while ((opt = getopt(argc, argv, "b:")) != -1) {
		switch (opt) {
		case 'b':
			bufsize = atoi(optarg);
			break;
		case '?':
			usage(argv[0]);
			break;
		default:
			assert(0);
		}
	}

	while (argv[optind] != NULL) {
		do_cat(argv[optind], bufsize);
		++optind;
	}

	/* 0 if all went OK, 1 if we hit an error. */
	return (ret);
}
