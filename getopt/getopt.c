/*
 * An example on getopt(). See getopts.sh which is a shell counterpart to this
 * C code.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>

void
usage(char *argv0)
{
	fprintf(stderr,
	    "usage: %s command [-c code] [filename [filename [...]]]\n",
	    argv0);

	exit(1);
}

int
main(int argc, char **argv)
{
	int opt;
	char *argv0 = basename(argv[0]);

	/* Comment out to print getopt() error messages. */
	opterr = 0;

	/* 1st param always required. */
	if (argc < 2)
		usage(argv0);

	printf("first param (command): %s\n", argv[1]);

	argv++;
	argc--;
	while ((opt = getopt(argc, argv, "c:")) != -1) {
		switch (opt) {
		case 'c':
			printf("option -c set to '%s'\n", optarg);
			break;
		case '?':
			printf("unknown option: '%c'\n", optopt);
			usage(argv0);
			break;
		}
	}

	printf("...done reading option arguments\n");

	/* optind is the 1st non-option argument */
	argv += optind;
	argc -= optind;

	if (argc >= 1) {
		printf("filenames:");
		while (argv[0] != NULL) {
			printf(" %s", argv[0]);
			++argv;
		}
		printf("\n");
	} else {
		printf("no filenames entered\n");
	}

	return (0);
}
