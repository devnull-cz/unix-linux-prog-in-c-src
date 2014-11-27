/*
 * Example on file locking.  The program creates NPROC processes.  Each child
 * goes through the file's first 70 bytes and sets the position for every
 * consecutive byte before writing its id character to it (see array 'c').  When
 * the middle is reached, if -l/-L is used, get the lock.  In other words, first
 * half of the file is always written into without any synchronization between
 * the processes, the other half may be writable to one process only and should
 * contain only one of possible NPROC characters if written and read under the
 * lock.  Only the -L solution actually works correctly though.
 *
 * The file middle is denoted via '|' character.
 *
 * You have three ways of running it.
 *
 * (1) The following will not lock at all:
 *
 *   terminal1$ ./a.out xxx
 *   terminal2$ gcc -o reader reader.c
 *   terminal2$ ./reader -l xxx
 *
 * You could also use the shell to print the file without locking on read:
 *
 *   terminal2$ while :; do cat xxx; printf "\r"; done
 *
 * You will see something like this since there was no locking:
 *
 *	_###./#./////_///.__#_____/_#___._#_|####_//..._#/.##._#.../..#####__#
 *
 * (2) Will use locking for the 2nd half of the file with descriptor sharing
 * (that's the wrong solution, see "NOTE" below in the code):
 *
 *   terminal1$ ./a.out -l xxx
 *   terminal2$ gcc -o reader reader.c
 *   terminal2$ ./reader -l xxx
 *
 * You will see something "better" but obviously it does not work correctly.
 * The first half sees contention which is fine but the 2nd half should have
 * been composed from the same character since only one process is allowed to
 * write to it and the reader acquired the lock (drop -l to see what happens if
 * we read it without the lock):
 *
 *	__///////____+_+_+++_______++_++___|///_////////////////_/////////////
 *
 * (3) Will use locking for the 2nd half of the file without descriptor sharing:
 *
 *   terminal1$ ./a.out -L xxx
 *   terminal2$ gcc -o reader reader.c
 *   terminal2$ ./reader -l xxx
 *
 * You should see something like the following output.  Note that the 1st half
 * is not much random because of the scheduler and a side effect of the implicit
 * ordering based on synchronization on the file middle position.  The important
 * thing though is that the 2nd half does consist of one character only while
 * the 1st does not.  Also, try to use the reader without the -l option to see
 * what happens.
 *
 *	########_________________________..|##################################
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

#define	FILE_LEN	70
/* if you change this you MUST change the 'c' array below */
#define	NPROC		5

/* define DEBUG to get some debugging printf's */
#undef DEBUG

void
sigint_handler(int sig)
{
	/* kill the whole group on Ctrl-C */
	kill(SIGTERM, -1);

	/* should not be neccessary */
	_exit(0);
}

char c[NPROC] = {'/', '_', '#', '+', '.'};

int
main(int argc, char **argv)
{
	struct sigaction act;
	int i, j, fd, locking = 0;
	struct flock fl;

	if (argc < 2 || argc > 3)
		errx(1, "usage: %s <-l|-L> <filename>", argv[0]);

	act.sa_handler = sigint_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	(void) sigaction(SIGINT, &act, NULL);

	/* simple way of processing one optional argument */
	if (strcmp(argv[1], "-l") == 0) {
		(void) printf("will use locking with shared fd...\n");
		locking = 1;
		++argv;
	} else if (strcmp(argv[1], "-L") == 0) {
		(void) printf("will use locking with private fd's...\n");
		locking = 2;
		++argv;
	} else if (argc > 2) {
		(void) printf("use -l or -L\n");
		exit(1);
	}

	/* Create the file. */
	if ((fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1)
		err(1, "open");
	if (locking == 2)
		close(fd);

	/* extend the file to FILE_LEN bytes */
	(void) lseek(fd, SEEK_SET, FILE_LEN - 1);
	(void) write(fd, "", 1);

	/* set the structure */
	fl.l_whence = SEEK_SET;
	fl.l_start = FILE_LEN / 2;
	fl.l_len = FILE_LEN / 2;

	/* create some processes */
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
			if (j == FILE_LEN / 2) {
				if (locking > 0) {
					fl.l_type = F_WRLCK;
					if (fcntl(fd, F_SETLKW, &fl) == -1)
						err(1, "fcntl");
				}
				/* Mark the 2nd half of the file. */
				(void) lseek(fd, j++, SEEK_SET);
				(void) write(fd, "|", 1);
			}

#if defined(DEBUG)
			if (j >= FILE_LEN / 2) {
				(void) printf("%c[%d]", c[i], j);
				(void) fflush(stdout);
			}
#endif
			/*
			 * NOTE: If locking is set to 1, the file position
			 *	 is shared among the NPROC processes so it could
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
			(void) lseek(fd, j, SEEK_SET);
			++j;
			(void) write(fd, c + i, 1);

			/*
			 * If we reached the end of file release the lock if we
			 * used locking (-l or -L).  Restart the counter so the
			 * next iteration starts from the beginning of the file
			 * again.
			 */
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
		(void) wait(NULL);

	return (0);
}
