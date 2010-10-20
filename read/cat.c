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
do_cat(char *file, int blocksize)
{
	int fd, n;
	char *buf = NULL;

	/* Bail out right away, we cannot proceed with no memory left. */
	if ((buf = malloc(blocksize)) == NULL)
		err(1, "malloc");

	if ((fd = open(file, O_RDONLY)) == -1) {
		warn("open");
		ret = 1;
		return;
	}

	while ((n = read(fd, buf, blocksize)) > 0) {
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
	int i, opt, blocksize = 4096;

	if (argc == 1)
		errx(1, "usage: %s [-b blocksize] <file> [<file2> ...]",
		    basename(argv[0]));
 
	while ((opt = getopt(argc, argv, "b:")) != -1) {
		switch(opt) {
		case 'b':
			blocksize = atoi(optarg);
			break;
		case '?':
			errx(1, "usage: %s [-b blocksize] <file> [<file2> ...]",
			    basename(argv[0]));
			break;
		}
	}

	i = optind;
	while (argv[i] != NULL) {
		do_cat(argv[i], blocksize);
		++i;
	}

	/* 0 if all went OK, 1 if we hit an error. */
	return (ret);
}
