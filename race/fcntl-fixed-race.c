/*
 * See race.c for more information. This version uses fcntl() for
 * synchronization. Note that most of the time now is spent in those
 * synchronization functions (when you kill it with Ctrl-C you see how many
 * loops were done. Try race.c and this and compare).
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

unsigned long i;	/* number of loops per process */
			/* u_long should be enough for basic demo */
unsigned int j;		/* j is number of races detected */
volatile sig_atomic_t run = 1;

static void
print_stats(void)
{
	printf("\nstats: inconsistency %u of %lu\n", j, i);
}

void
finish(int sig)
{
	run = 0;
}

#define	LOCK(fd, fl)	lock_unlock(fd, -1, fl)
#define	UNLOCK(fd, fl)	lock_unlock(fd, 1, fl)

/* lock_unlock(1) means unlock, lock_unlock(-1) is lock */
static void
lock_unlock(int fd, int n, struct flock *fl)
{
	switch (n) {
	case 1:
		fl->l_type = F_UNLCK;
		break;
	case -1:
		fl->l_type = F_WRLCK;
		break;
	default:
		errx(1, "incorrect use of lock_unlock");
	}

	if (fcntl(fd, F_SETLKW, fl) == -1) {
		if (errno == EINTR)
			print_stats();
		err(1, "fcntl %s", fl->l_type == F_UNLCK ? "unlock" : "lock");
	}
}

int
main(int argc, char **argv)
{
	char c = 0;
	int fd, dbg = 0;
	char *addr = NULL;
	struct sigaction act;
	/* l_len is not relevant since the locking is used as bracketing. */
	struct flock fl = { .l_whence = SEEK_SET, .l_start = 0, .l_len = 2 };

	if (argc == 1)
		printf("run with any argument to see some debug info\n");
	else
		dbg = 1;

	memset(&act, '\0', sizeof (act));
	act.sa_handler = finish;
	sigaction(SIGINT, &act, NULL);

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
		err(1, "test.dat create");

	/* extend the file to 2 bytes */
	write(fd, &c, 1);
	write(fd, &c, 1);

	addr = mmap(0, 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((void *)addr == MAP_FAILED)
		err(1, "mmap");

	switch (fork()) {
	case -1:
		err(1, "fork");
	case 0:
		while (run) {
			LOCK(fd, &fl);
			if (addr[0] != addr[1]) {
				if (dbg) {
					fprintf(stderr, "[child (%d/%d)] ",
					    addr[0], addr[1]);
				}
				++j;
			}
			addr[0] = addr[1] = 2;
			UNLOCK(fd, &fl);
			++i;
		}
		break;
	default:
		while (run) {
			LOCK(fd, &fl);
			if (addr[0] != addr[1]) {
				if (dbg) {
					fprintf(stderr, "[PARENT (%d/%d)] ",
					    addr[0], addr[1]);
				}
				++j;
			}
			addr[0] = addr[1] = 1;
			UNLOCK(fd, &fl);
			++i;
		}
		wait(NULL);
		break;
	}

	munmap(addr, 2);
	close(fd);

	print_stats();

	return (0);
}
