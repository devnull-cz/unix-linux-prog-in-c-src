/*
 * An example on getopt(). See getopts.sh which is a shell counterpart to this
 * C code.
 *
 * Run e.g. like this:
 *   ./a.out foo -c aaa bar "har haha"
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#define	_XOPEN_SOURCE 700	// needed for F_LOCK

#include <err.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
usage(char *argv0)
{
	errx(1, "usage: %s <command> [-c code] <filename> [<filename> [...]]",
	    argv0);
}

int
main(int argc, char **argv)
{
	int opt;
	char *argv0 = basename(argv[0]);
	char *code = NULL;

	/* Comment out to print getopt() error messages. */
	opterr = 0;

	/* 1st param always required. */
	if (argc < 2)
		usage(argv0);

	printf("first param (command): %s\n", argv[1]);
	argv++;
	argc--;

	/*
	 * With leading ':', we can distinguish between an unknown option and a
	 * missing argument to a known option.
	 */
	while ((opt = getopt(argc, argv, ":c:")) != -1) {
		switch (opt) {
		case 'c':
			/*
			 * optarg should be copied since it might be
			 * overwritten by another option or freed by getopt()
			 */
			if ((code = strdup(optarg)) == NULL)
				err(1, "cannot alloc memory for -c optarg");
			break;
		case ':':
			fprintf(stderr, "missing argument for -%c\n", optopt);
			usage(argv0);
			break;
		case '?':
			fprintf(stderr, "unknown option -%c\n", optopt);
			usage(argv0);
			break;
		}
	}

	printf("...done reading option arguments\n");

	if (code != NULL)
		printf("option -c set to '%s'\n", code);

	/* optind is the 1st non-option argument. */
	argv += optind;
	argc -= optind;

	if (argc == 0) {
		printf("no filenames entered\n");
		return (0);
	}

	printf("filenames:");
	while (argv[0] != NULL) {
		printf(" '%s'", argv[0]);
		++argv;
	}
	printf("\n");
}
