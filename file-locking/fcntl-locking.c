/*
 * Example on file locking.  The program creates NPROC processes.  Each child
 * goes through the file's first 70 bytes and sets the position for every
 * consecutive byte before writing its ID character mark to it (see the 'c'
 * array).  When the middle of the line is reached, if -l/-L is used, get the
 * lock.  In other words, first half of the file is always written into without
 * any synchronization between the processes, the other half is writable
 * optionally to one process only and should contain only one of possible NPROC
 * characters if written and read under the lock.  Only the -L solution actually
 * works correctly though.
 *
 * The file middle is denoted via '|' character.
 *
 * You have three ways of running it.  Use the 'reader' command to print the
 * present state of the line while holding a lock.
 *
 * (1) The following will not lock at all:
 *
 *   terminal1$ ./fcntl-locking xxx
 *
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
 *   terminal1$ ./fcntl-locking -l xxx
 *
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
 *   terminal1$ ./fcntl-locking -L xxx
 *
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

#define	_XOPEN_SOURCE 700	// needed for kill()/sigaction

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

static void
usage(char *progname)
{
		errx(1, "usage: %s [-l|-L] <filename>", progname);
}

int
main(int argc, char **argv)
{
	struct sigaction act;
	int i, j, fd, locking = 0;
	struct flock fl;
	char *progname = argv[0];

	act.sa_handler = sigint_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	(void) sigaction(SIGINT, &act, NULL);

	int r;
	while ((r = getopt(argc, argv, "lL")) != -1) {
		switch (r) {
		case 'l':
			(void) printf("Using locking with a shared fd.\n");
			locking = 1;
			break;
		case 'L':
			(void) printf("Using locking with private fd's.\n");
			locking = 2;
			break;
		default:
			usage(progname);
			break;
		}
	}
	argv += optind;
	argc -= optind;

	if (argc != 1)
		usage(progname);

	if (locking == 0)
		printf("Not using locking.\n");

	/* Create the file. */
	char *filename = argv[0];
	if ((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1)
		err(1, "open");
	if (locking == 2)
		close(fd);

	/* extend the file to FILE_LEN bytes */
	(void) lseek(fd, SEEK_SET, FILE_LEN - 1);
	(void) write(fd, "", 1);

	fl.l_whence = SEEK_SET;
	fl.l_start = FILE_LEN / 2;
	fl.l_len = FILE_LEN / 2;

	/* create some processes */
	for (i = 0; i < NPROC; ++i) {
		/* parent */
		if (fork() != 0)
			continue;

		/* child */
		if (locking == 2) {
			if ((fd = open(filename, O_WRONLY, 0666)) == -1)
				err(1, "open");
		}

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
			 *	 is shared among the NPROC processes so the
			 *	 following could happen (timewise):
			 *
			 *	   1) process in the upper half lseek()s to
			 *	      offset > FILE_LEN / 2
			 *	   2) a process in the lower half lseek()s to
			 *	      offset < FILE_LEN / 2
			 *	   3) the process in the upper half write()s
			 *	      UNDER THE LOCK BUT WRITES TO THE LOWER
			 *	      HALF!
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

	/* Not reached. */
	for (i = 0; i < NPROC; ++i)
		(void) wait(NULL);
}
