/*
 * Be careful with zeroes. Try this on a *local* filesystem (typically not your
 * home directory in the lab):
 *
 * cc big-file.c
 * ./a.out
 * du -h BIG
 * gzip BIG
 * du -h BIG.gz
 * gunzip BIG.gz
 * du -h BIG
 */

#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int
main(void)
{
	int fd;

	if ((fd = open("BIG", O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1)
		err(1, "open");

	lseek(fd, 1000000, SEEK_SET);
	write(fd, "", 1);
	close(fd);

	return (0);
}
