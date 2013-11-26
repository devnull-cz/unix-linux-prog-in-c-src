/*
 * Rather stupid example on file locking. Run it like this:
 *
 * (a) ./a.out xxx		# will not lock
 * (b) ./a.out -l xxx		# will use locking for the 2nd half of the file
 *
 * And from the 2nd terminal, see the file contents:
 *
 *	$ while :; do cat xxx; printf "\r"; done
 *
 * In the 1st example, you might see just "cccc....ccccc" which is because of
 * the scheduling and the ordering of the processes - the last one always
 * rewrites each character with 'c' before all of them wait 10 miliseconds so it
 * looks like we have just 'c's there.
 *
 * In the 2nd example, you will see that the processes synchronize in the middle
 * of the file.
 *
 * (c) jp@devnull.cz
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

void
sigint_handler(int sig)
{
	/* kill the whole group on Ctrl-C */
	kill(SIGTERM, -1);

	/* should not be neccessary */
	_exit(0);
}

char c[3] = {'a', 'b', 'c'};

int
main(int argc, char **argv)
{
	struct sigaction act;
	int i, j, fd, locking = 0;
	struct flock fl;

	if (argc < 2 || argc > 3)
		errx(1, "usage: %s [-l] <filename>", argv[0]);

	act.sa_handler = sigint_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);

	/* simple way of processing one optional argument */
	if (strcmp(argv[1], "-l") == 0) {
		printf("will use locking...\n");
		locking = 1;
		++argv;
	}

	if ((fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1)
		err(1, "open");

	/* extend the file to FILE_LEN bytes */
	lseek(fd, SEEK_SET, FILE_LEN - 1);
	write(fd, "", 1);

	/* set the structure */
	fl.l_whence = SEEK_SET;
	fl.l_start = FILE_LEN / 2;
	fl.l_len = FILE_LEN / 2;

	/* create 3 processes */
	for (i = 0; i < 3; ++i) {

		/* parent */
		if (fork() != 0)
			continue;

		/* child */
		j = 0;
		while (1) {
			/* lock only the 2nd half of the file */
			if (locking == 1 && j == FILE_LEN / 2) {
				fl.l_type = F_WRLCK;
				if (fcntl(fd, F_SETLKW, &fl) == -1)
					err(1, "fcntl");
			}

			lseek(fd, j, SEEK_SET);
			++j;
			write(fd, c + i, 1);

			/* Use poll() as a trick to sleep for 10 ms. */
			poll(NULL, 0, 10);

			if (j == FILE_LEN) {
				j = 0;
				if (locking == 1) {
					fl.l_type = F_UNLCK;
					if (fcntl(fd, F_SETLKW, &fl) == -1)
						err(1, "fcntl");
				}
			}
		}
	}

	/* not reached... */
	for (i = 0; i < 3; ++i)
		wait(NULL);

	return (0);
}
