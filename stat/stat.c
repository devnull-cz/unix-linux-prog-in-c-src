/*
 * Simple example on stat(2) call.
 *
 * On Solaris, to create for example a 1MB file is simple: "mkfile 1m big".
 * Alternatively, use dd(1) with /dev/zero as a data source.
 */
#include <sys/stat.h>
#include <sys/types.h>

#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int fd, ret;
	struct stat buf;

	if (argc != 2)
		errx(1, "Filename missing as a parameter.");

	if ((fd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open");

	ret = fstat(fd, &buf);
	assert(ret == 0);

	/*
	 * There is no a truly good way how to print most of the members of the
	 * stat struct.  (u)intmax_t and PRI(d|u)MAX (from inttypes.h) might be
	 * the best way (albeit far from optimal) how to deal with that.  See
	 * the 'j' modifier below.
	 */

	/*
	 * Verify with "POSIXLY_CORRECT=1 du -s <file>"
	 * (The env var is there for GNU dd(1) to work correctly in 512 byte
	 * blocks otherwise it would print out size in 1024 byte blocks.)
	 *
	 * You can also use "du -B 512 -s <file>"
	 */
	printf("Allocated number of 512 byte blocks: %jd\n",
	    (intmax_t)buf.st_blocks);
	/* Verify with "ls -i <file>" */
	printf("Inode number: %lu\n", (long)buf.st_ino);

	return (0);
}
