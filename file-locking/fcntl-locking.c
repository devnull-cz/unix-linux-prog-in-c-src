/*
 * Example on file locking. Run it like either this:
 *
 * Will not lock:
 *
 *   terminal1$ ./a.out xxx
 *   terminal2$ while :; do cat xxx; printf "\r"; done
 *
 * Will use locking for the 2nd half of the file with descriptor sharing:
 *
 *   terminal1$ ./a.out -l xxx
 *   terminal2$ gcc -o reader reader.c
 *   terminal2$ ./reader -l xxx
 *
 * Will use locking for the 2nd half of the file with descriptor sharing:
 *
 *   terminal1$ ./a.out -L xxx
 *   terminal2$ gcc -o reader reader.c
 *   terminal2$ ./reader -l xxx
 *
 * In the 1st example, you might see just line filled with the same character
 * which is because of the scheduling and the ordering of the processes -
 * the last one always rewrites each character with 'c' before all of them
 * wait 10 miliseconds so it looks like we have just single character there.
 *
 * In the 2nd example, you will see that the processes synchronize
 * in the middle of the file which however does not work since the processes
 * in lower half of the file steal position in the file from the process
 * in the upper half.
 *
 * The 3rd example is correct way how to deal with this.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
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
#include <assert.h>

#define	FILE_LEN 70
#define	NPROC	3

void
sigint_handler(int sig)
{
	/* kill the whole group on Ctrl-C */
	kill(SIGTERM, -1);

	/* should not be neccessary */
	_exit(0);
}

char c[3] = {'/', '_', '#'};

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
		printf("will use locking with shared fd...\n");
		locking = 1;
		++argv;
	} else if (strcmp(argv[1], "-L") == 0) {
		printf("will use locking with private fd's...\n");
		locking = 2;
		++argv;
	}

	if (locking == 1) {
		if ((fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY,
		    0666)) == -1)
			err(1, "open");
	}

	/* extend the file to FILE_LEN bytes */
	lseek(fd, SEEK_SET, FILE_LEN - 1);
	write(fd, "", 1);

	/* set the structure */
	fl.l_whence = SEEK_SET;
	fl.l_start = FILE_LEN / 2;
	fl.l_len = FILE_LEN / 2;

	/* create 3 processes */
	for (i = 0; i < NPROC; ++i) {

		/* parent */
		if (fork() != 0)
			continue;

		if (locking == 2) {
			if ((fd = open(argv[1], O_WRONLY, 0666)) == -1)
				err(1, "open");
		}

		/* child */
		j = 0;
		while (1) {

			/* Lock only the 2nd half of the file. */
			if (locking > 0 && j == FILE_LEN / 2) {
				fl.l_type = F_WRLCK;
				if (fcntl(fd, F_SETLKW, &fl) == -1)
					err(1, "fcntl");

				/* Mark the 2nd half of the file. */
				lseek(fd, j++, SEEK_SET);
				write(fd, "|", 1);
			}

			if (j >= FILE_LEN / 2) {
				printf("%c[%d]", c[i], j);
				fflush(stdout);
			}
			/*
			 * NOTE: If locking is set to 1, the file position
			 *	 is shared among the 3 processes so it could
			 *	 happen that (timewise):
			 *
			 *	   1) process in upper half lseek()'s
			 *	      to offset > FILE_LEN / 2
			 *	   2) a process in lower half lseek()'s
			 *	      to offset < FILE_LEN / 2
			 *	   3) the process in upper half write()'s
			 *	      BUT THE WRITE IS DONE TO LOWER HALF !
			 *
			 *	 This is thanks to scheduling and the fact
			 *	 that lseek() does not consult any locks.
			 */
			lseek(fd, j, SEEK_SET);
			++j;
			write(fd, c + i, 1);

			/* Use poll() as a trick to sleep for 10 ms. */
			poll(NULL, 0, 10);

			if (j == FILE_LEN) {
				j = 0;
				if (locking > 0) {
					fl.l_type = F_UNLCK;
					if (fcntl(fd, F_SETLKW, &fl) == -1)
						err(1, "fcntl");
				}
			}
		}
	}

	/* not reached... */
	for (i = 0; i < NPROC; ++i)
		wait(NULL);

	return (0);
}
