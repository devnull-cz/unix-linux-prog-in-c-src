/*
 * Be careful with zeroes.  Try this on a *local* filesystem (typically not your
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

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	int fd;

	if ((fd = open("BIG", O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1)
		err(1, "open");

	/* 1MB */
	lseek(fd, 1000000, SEEK_SET);
	write(fd, "", 1);
	close(fd);

	return (0);
}
