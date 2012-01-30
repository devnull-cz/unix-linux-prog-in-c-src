/*
 * Very simple cat(1)-like program. Example program on open/read/write/close
 * system calls.
 *
 * (c) jp@devnull.cz
 */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <err.h>

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
	free(buf);
}

int
main(int argc, char **argv)
{
	int i, opt, bufsize = 4096;

	if (argc == 1)
		errx(1, "usage: %s [-b bufsize] <file> [<file2> ...]",
		    basename(argv[0]));

	while ((opt = getopt(argc, argv, "b:")) != -1) {
		switch (opt) {
		case 'b':
			bufsize = atoi(optarg);
			break;
		case '?':
			errx(1, "usage: %s [-b bufsize] <file> [<file2> ...]",
			    basename(argv[0]));
			break;
		}
	}

	i = optind;
	while (argv[i] != NULL) {
		do_cat(argv[i], bufsize);
		++i;
	}

	/* 0 if all went OK, 1 if we hit an error. */
	return (ret);
}
