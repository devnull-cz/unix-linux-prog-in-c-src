/*
 * See race.c and sem-fixed-race.c for more information.
 *
 * This version uses POSIX semaphores for synchronization.  After you run this
 * and kill it with Ctrl-C you see some statistics as to how many loops were
 * run.  Compare with implementations mentioned above.  You will see POSIX
 * semaphores are an order of magnitude more efficient than Sys V semaphores.
 *
 * You must link with -pthread on Linux.
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define	SEM_NAME	"/mysem"

sem_t *sem;
int fd, parent;
char *addr = NULL;
unsigned long loops_done;
unsigned int races_detected;
volatile sig_atomic_t run = 1;

static void
print_stats(void)
{
	printf("\nstats: inconsistencies %u out of %lu\n", races_detected,
	    loops_done);
}

void
finish(int sig)
{
	run = 0;
}

static void
cleanup(void)
{
	/* You should check the return values here for all the calls. */
	(void) sem_close(sem);
	if (parent)
		(void) sem_unlink(SEM_NAME);

	(void) munmap(addr, 2);
	close(fd);
}

int
main(int argc, char **argv)
{
	char c = 0;
	int dbg = 1;
	pid_t pid, ret_pid;
	struct sigaction act = { 0 };

	if (argc == 1) {
		printf("Run with any argument to see some debugging info.\n");
		dbg = 0;
	}

	/* Get a semaphore. */
	sem = sem_open(SEM_NAME, O_CREAT, S_IRWXU, 1);
	if (sem == SEM_FAILED)
		err(1, "sem_open");

	act.sa_handler = finish;
	sigaction(SIGINT, &act, NULL);

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
		err(1, "open");

	/* extend the file to 2 bytes */
	write(fd, &c, 1);
	write(fd, &c, 1);

	addr = mmap(0, 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
		err(1, "mmap");

	if ((pid = fork()) == -1)
		err(1, "fork");

	/* Child */
	if (pid == 0) {
		while (run) {
			sem_wait(sem);
			if (addr[0] != addr[1]) {
				if (dbg) {
					fprintf(stderr, "[child (%d/%d)] ",
					    addr[0], addr[1]);
				}
				++races_detected;
			}
			addr[0] = addr[1] = 2;
			sem_post(sem);
			++loops_done;
		}
		print_stats();
		cleanup();
		return (0);
	}

	/* Parent only. */
	assert(pid > 0);
	parent = 1;
	while (run) {
		sem_wait(sem);
		if (addr[0] != addr[1]) {
			if (dbg) {
				fprintf(stderr, "[PARENT (%d/%d)] ",
				    addr[0], addr[1]);
			}
			++races_detected;
		}
		addr[0] = addr[1] = 1;
		sem_post(sem);
		++loops_done;
	}
	ret_pid = waitpid(pid, NULL, 0);
	assert(ret_pid == pid);

	print_stats();
	cleanup();

	return (0);
}
