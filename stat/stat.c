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
	/* Verify with "du -s <file>" */
	printf("Allocated blocks: %ld\n", buf.st_blocks);
	/* Verify with "ls -i <file>" */
	printf("Inode number: %ld\n", buf.st_ino);

	return (0);
}
