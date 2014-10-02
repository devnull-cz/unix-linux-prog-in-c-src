/*
 * Demonstrate mandatory locking by locking the whole file.
 *
 * Run like this:
 *
 *   cp /etc/passwd mandatory.txt
 *
 *   # Solaris
 *   chmod 2660 mandatory.txt
 *   # Linux
 *   chmod g-x,g+s mandatory.txt
 *
 *   gcc lockf.c
 *   ./a.out mandatory.txt &
 *
 *   # in new terminal run this (the command should hang or the file-system
 *   # does not support or was mounted without mandatory locking):
 *   echo foo >> mandatory.txt
 *
 *   # in the original terminal do this:
 *   cat mandatory.txt
 *   kill $!
 *   cat mandatory.txt
 *
 * vlada@devnull.cz
 */

#include <stdio.h>
#ifndef NOERR
#include <err.h>
#else
#define	err(n, ...)	printf(__VA_ARGS__)
#define	errx(n, ...)	printf(__VA_ARGS__)
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	int fd;
	off_t size;

	if (argc != 2)
		errx(1, "usage: %s <file>", argv[0]);

	if ((fd = open(argv[1], O_RDWR)) == -1)
		err(1, "open");

	if ((size = lseek(fd, 0, SEEK_END)) == -1)
		err(1, "lseek");

	if (lockf(fd, F_LOCK, size) == -1)
		err(1, "lockf");

	pause();

	return (0);
}
