/*
 * Demonstrate mandatory locking by locking the whole file.
 *
 * Run like this:
 *
 *   cp /etc/passwd /var/tmp/mandatory.txt
 *
 *   # Solaris
 *   chmod 2660 /var/tmp/mandatory.txt
 *   # Linux
 *   chmod g-x,g+s /var/tmp/mandatory.txt
 *
 *   gcc lockf.c
 *   ./a.out /var/tmp/mandatory.txt &
 *
 *   # in new terminal run this:
 *   echo foo >> /var/tmp/mandatory.txt
 *   cat /var/tmp/mandatory.txt
 *
 *   # in the original terminal do this:
 *   kill $!
 *   cat /var/tmp/mandatory.txt
 *
 * NOTE: if the file-system does not support or was mounted without
 *	 mandatory locking the commands in the new terminal will be
 *	 able to read/write to the file.
 *
 * vlada@devnull.cz
 */

#define	_XOPEN_SOURCE 700	// needed for F_LOCK

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

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
