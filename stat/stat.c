/*
 * Simple example on stat(2) call.
 *
 * On Solaris, to create for example a 1MB file is simple: "mkfile 1m big".
 * Alternatively, use dd(1) with /dev/zero as a data source.
 */
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int
main(int argc, char **argv)
{
	int fd;
	struct stat buf;

	if (argc != 2)
		errx(1, "Filename missing as a parameter.");

	if ((fd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open");

	fstat(fd, &buf);

	/*
	 * There is no truly good way how to print most of the members of struct
	 * stat. For the following prints (u)intmax_t and PRI(d|u)MAX (from
	 * inttypes.h) might be the best way (albeit far from optimal) how to
	 * deal with that.
	 */

	/*
	 * Verify with "POSIXLY_CORRECT=1 du -s <file>"
	 * (The env var is there for GNU dd(1) to work correctly in 512 byte
	 * blocks otherwise it would print out size in 1024 byte blocks.)
	 */
	printf("Allocated number of 512 byte blocks: %ld\n", buf.st_blocks);
	/* Verify with "ls -i <file>" */
	printf("Inode number: %lu\n", (long) buf.st_ino);

	return (0);
}
