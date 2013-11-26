/*
 * Reader
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

#define	FILE_LEN 70

int
main(int argc, char **argv)
{
	int i, j, fd, locking = 0;
	struct flock fl;
	char buf[FILE_LEN];

	if (argc < 2 || argc > 3)
		errx(1, "usage: %s [-l] <filename>", argv[0]);

	/* simple way of processing one optional argument */
	if (strcmp(argv[1], "-l") == 0) {
		printf("will use locking...\n");
		locking = 1;
		++argv;
	}

	// if ((fd = open(argv[1], O_RDWR)) == -1)
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open");

	/* set the structure */
	fl.l_whence = SEEK_SET;
	fl.l_start = FILE_LEN / 2;
	fl.l_len = FILE_LEN / 2;

	while (1) {
		/* lock only the 2nd half of the file */
		if (locking == 1) {
			fl.l_type = F_RDLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1)
				err(1, "fcntl");
		}

		printf("Got the lock now\n");
		lseek(fd, SEEK_SET, 0);
		read(fd, buf, sizeof (buf));
		write(1, buf, sizeof (buf));
		// write(1, "\r", 1);

		/* Use poll() as a trick to sleep for 10 ms. */
		// poll(NULL, 0, 10);
		sleep (1);
		printf("\nReleasing the lock\n");

		if (locking == 1) {
			fl.l_type = F_UNLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1)
				err(1, "fcntl");
		}

		/* let the writers do their job */
		sleep(1);
	}

	/* notreached */
	return (0);
}
