/*
 * Open a file, set a position, and then move from that position. Read one
 * character from there. Proves that you can use a negative number for the
 * offset used with SEEK_CUR.  Remember that the first byte is on position 0.
 *
 * If the file contains "123456....", then:
 *
 *	  - "./a.out file 3 0" prints 4
 *	  - "./a.out file 3 -1" prints 3
 *	  - "./a.out file 0 -1" prints an error since we cannot move before the
 *	    beginning of the file
 */
#include <err.h>
#include <err.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	char c;
	int fd;
	ssize_t n;

	if (argc != 4) {
		errx(1, "usage: %s <file> <position> <offset-from-position>",
		    basename(argv[0]));
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open");

	if (lseek(fd, atoi(argv[2]), SEEK_SET) == -1)
		err(1, "lseek");

	if (lseek(fd, atoi(argv[3]), SEEK_CUR) == -1)
		err(1, "lseek");

	if ((n = read(fd, &c, 1)) != 1) {
		if (n == 0)
			errx(1, "Tried to read beyond the end of the file.");
		err(1, "read");
	}

	printf("Character read: '%c'\n", c);
}
