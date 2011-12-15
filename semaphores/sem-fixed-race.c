/*
 * See race/race.c for more information. This version uses semaphores for
 * synchronization. Note that most of the time now is spent in those
 * synchronization functions (when you kill it with Ctrl-C you see how much
 * loops was done. Try race.c and this and compare).
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <err.h>

unsigned long i;	/* number of loops per process, u_long should be enough for basic demo */
unsigned int j;		/* j is number of races detected */

void
print_stat(int sig)
{
	printf("\nstats: inconsistency %u of %lu\n", j, i);
	_exit(0);
}

int sem;
struct sembuf down = { 0, -1, 0};
struct sembuf up = { 0, 1, 0};

/* up_down(1) means UP, up_down(-1) is DOWN */
int
up_down(int n)
{
	if (n == 1) {
		if (semop(sem, &down, 1) == -1)
			err(1, "semop up");
	}
	else if (n == -1) {
		if (semop(sem, &up, 1) == -1)
			err(1, "semop down");
	}
	else
		errx(1, "incorrect use of up_down");
}

int
main(int argc, char **argv)
{
	key_t key;
	char c = 0;
	int fd, dbg = 0;
	char *addr = NULL;
	struct sigaction act;
	if (argc == 1)
		printf("run with any argument to see some debug info\n");
	else
		dbg = 1;

	/* get a semaphore */
	key = ftok("/etc/passwd", 0);
	if ((sem = semget(key, 1, IPC_CREAT | S_IRUSR | S_IWUSR)) == -1)
		err(1, "semget");

	/* initialize it */
	if (semctl(sem, 0, SETVAL, 1) == -1)
		err(1, "semctl");

	bzero(&act, sizeof(act));
	act.sa_handler = print_stat;
	sigaction(SIGINT, &act, NULL);

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
		err(1, "open");

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
			up_down(-1);
			if (addr[0] != addr[1]) {
				if (dbg)
					fprintf(stderr, "[child (%d/%d)] ",
					    addr[0], addr[1]);
				++j;
			}
			addr[0] = addr[1] = 2;
			up_down(1);
			++i;
		}
		break;
	default:
		while (1) {
			up_down(-1);
			if (addr[0] != addr[1]) {
				if (dbg)
					fprintf(stderr, "[PARENT (%d/%d)] ",
					    addr[0], addr[1]);
				++j;
			}
			addr[0] = addr[1] = 1;
			up_down(1);
			++i;
		}
		break;
	}

	munmap(addr, 2);
	close(fd);

	return (0);
}
