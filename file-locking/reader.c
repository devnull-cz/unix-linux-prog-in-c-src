/*
 * Reader for fcntl-locking.c example.
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <sys/wait.h>
#include <strings.h>

#define	FILE_LEN 70	/* should match the constant in fcntl-locking.c */

int
main(int argc, char **argv)
{
	int fd, locking = 0;
	struct flock fl;
	char buf[FILE_LEN];
	int n;

	if (argc < 2 || argc > 3)
		errx(1, "usage: %s [-l] <filename>", argv[0]);

	/* simple way of processing one optional argument */
	if (strcmp(argv[1], "-l") == 0) {
		printf("will use locking...\n");
		locking = 1;
		++argv;
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open");

	/* Set the structure. */
	fl.l_whence = SEEK_SET;
	fl.l_start = FILE_LEN / 2;
	fl.l_len = FILE_LEN / 2;

	while (1) {
		/* Lock only the 2nd half of the file. */
		if (locking == 1) {
			fl.l_type = F_RDLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1)
				err(1, "fcntl");
		}

		(void) lseek(fd, SEEK_SET, 0);
		bzero(buf, sizeof (buf));
		n = read(fd, buf, sizeof (buf));
		(void) write(1, buf, sizeof (buf));
		(void) printf(" %d\n", n);

		sleep(1);
		if (locking == 1) {
			fl.l_type = F_UNLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1)
				err(1, "fcntl");
		}
		/* Let the writers do their job. */
		sleep(1);
	}

	/* not reached */
	return (0);
}
