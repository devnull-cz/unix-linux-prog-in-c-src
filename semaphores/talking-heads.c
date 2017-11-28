/*
 * <n> processes share one semaphore initialized with <init>.  Every moment,
 * <init> processes can talk at the same time.  When talking, each process say
 * its number every second for a random times seconds.  They keep doing this
 * until killed.  Every round, the last number is in () so that we know another
 * process is gonna get to talk.  As a special case, with <init> to be 1, only
 * one process at a time can talk:
 *
 *	$ ./a.out 9 1
 *	000(0)11(1)2(2)(3)444(4)5(5)(6)...
 *
 *	$ ./a.out 9 2
 *	010(1)(0)(2)34343(4)(3)5(1)(5)(6)707070(7)(0)28(2)84(8)...
 *
 * Catch SIGINT to close the semaphore (all processes) and unlink it (the parent
 * only).
 *
 * After each round, you might need a short sleep to give other processes a
 * chance to wake up otherwise only a small group of processes could actually
 * take turns (ie. if after sem_post() the process immediatelly do sem_wait(),
 * it itself may be the one that actually decrements the semaphore again).
 *
 * You must link with -pthread on Linux.
 *
 * (c) jp@devnull.cz
 */

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <poll.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define	SEM_NAME	"/mysem"
#define	MAXSECS		7

sem_t *sem;
int parent;
volatile sig_atomic_t run = 1;

void
finish(int sig)
{
	run = 0;
}

static void
cleanup(int id)
{
	/* You should check the return values here for all the calls. */
	(void) sem_close(sem);
	if (parent) {
		fprintf(stderr, "Cleaning up for parent.\n");
		(void) sem_unlink(SEM_NAME);
	} else {
		fprintf(stderr, "Cleaning up for child %d.\n", id);
	}
}

void
talk(int id)
{
	int i, nsecs;

	/* Loop at least once. */
	nsecs = rand() % (MAXSECS - 1) + 1;

	for (i = 0; i < nsecs; ++i) {
		/* Distinguish the last id print. */
		if (i == nsecs - 1)
			printf("(%d)", id);
		else
			printf("%d", id);
		fflush(stdout);
		sleep (1);
	}
}

int
main(int argc, char **argv)
{
	pid_t pid;
	int i, nproc, sinit;
	struct sigaction act = { 0 };

	if (argc != 3) {
		printf("Usage: %s <n> <initval>\n", basename(argv[0]));
		return (1);
	}

	nproc = atoi(argv[1]);
	if (nproc > 10) {
		printf("<n> can be up to 10 only.\n");
		return (1);
	}

	act.sa_handler = finish;
	sigaction(SIGINT, &act, NULL);

	sinit = atoi(argv[2]);
	if (sinit > nproc) {
		printf("<n> must not be less than <init>\n");
		return (1);
	}

	/* Get a semaphore. */
	printf("Initializing the semaphore to %d\n", sinit);
	sem = sem_open(SEM_NAME, O_CREAT, S_IRWXU, sinit);
	if (sem == SEM_FAILED)
		err(1, "sem_open");

	for (i = 0; i < nproc; ++i) {
		if ((pid = fork()) == -1)
			err(1, "fork");

		if (pid == 0) {
			printf("Child %d born (%d).\n", i, getpid());
			/* Child */
			srand(getpid());
			/* Desynchronize the output. */
			poll(NULL, 0, 100 * i);
			while (run) {
				sem_wait(sem);
				talk(i);
				sem_post(sem);
				/* Give other processes a chance to wake up. */
				sleep(1);
			}
			cleanup(i);
			return (0);
		}
	}

	/* Parent here. */
	parent = 1;
	for (i = 0; i < nproc; ++i)
		(void) wait(NULL);
	cleanup(0);

	return (0);
}
