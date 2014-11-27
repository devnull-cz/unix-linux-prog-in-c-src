/*
 * See race/race.c for more information. This version uses semaphores for
 * synchronization. Note that most of the time now is spent in those
 * synchronization functions (when you kill it with Ctrl-C you see how much
 * loops was done. Try race.c and this and compare).
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <signal.h>
#include <err.h>

unsigned long i;	/* number of loops per process */
			/* u_long should be enough for basic demo */
unsigned int j;		/* j is number of races detected */

void
print_stat(int sig)
{
	printf("\nstats: inconsistency %u of %lu\n", j, i);
	_exit(0);
}

/* lock_unlock(1) means unlock, lock_unlock(-1) is lock */
int
lock_unlock(int fd, int n, struct flock *fl)
{
	if (n == 1) {
		fl->l_type = F_UNLCK;
	} else if (n == -1) {
		fl->l_type = F_WRLCK;
	} else {
		errx(1, "incorrect use of lock_unlock");
	}

	if (fcntl(fd, F_SETLKW, fl) == -1)
		err(1, "fcntl %s", fl->l_type == F_UNLCK ? "unlock" : "lock");
}

int
main(int argc, char **argv)
{
	char c = 0;
	int fd, lockfd, dbg = 0;
	char *addr = NULL;
	struct sigaction act;
	struct flock fl = { .l_whence = SEEK_SET, .l_start = 0, .l_len = 1 };

	if (argc == 1)
		printf("run with any argument to see some debug info\n");
	else
		dbg = 1;

	bzero(&act, sizeof (act));
	act.sa_handler = print_stat;
	sigaction(SIGINT, &act, NULL);

	if ((lockfd = open("lockfile", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
		err(1, "lockfile create");

	write(lockfd, "", 1);

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
		while (1) {
			lock_unlock(lockfd, -1, &fl);
			if (addr[0] != addr[1]) {
				if (dbg)
					fprintf(stderr, "[child (%d/%d)] ",
					    addr[0], addr[1]);
				++j;
			}
			addr[0] = addr[1] = 2;
			lock_unlock(lockfd, 1, &fl);
			++i;
		}
		break;
	default:
		while (1) {
			lock_unlock(lockfd, -1, &fl);
			if (addr[0] != addr[1]) {
				if (dbg)
					fprintf(stderr, "[PARENT (%d/%d)] ",
					    addr[0], addr[1]);
				++j;
			}
			addr[0] = addr[1] = 1;
			lock_unlock(lockfd, 1, &fl);
			++i;
		}
		break;
	}

	munmap(addr, 2);
	close(fd);

	return (0);
}
