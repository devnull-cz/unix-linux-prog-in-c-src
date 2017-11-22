/*
 * POSIX semaphores demonstration. Note especially how sem_wait() should be
 * treated with regards to interruptions (like signals in this case or I/O
 * delivery).
 *
 * Run with and without argument to see the difference.
 *
 * Compile with:
 *   Linux:
 *     gcc -lpthread sem.c
 *   Solaris:
 *     cc sem.c
 *
 * vlada@devnull.cz, 2013
 */

#define	_XOPEN_SOURCE	700	// for sigaction()

#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <unistd.h>
#include <poll.h>
#include <err.h>

sem_t sem;

#define	PUTCHARF(x)	putc(x, stdout); fflush(stdout);

int
sem_wait_nointr(sem_t *s) {
	while (sem_wait(s) != 0) {
		if (errno != EINTR) {
			return (-1);
		} else {
			PUTCHARF('!');
		}
	}

	return (0);
}

void *
usher(void *x)
{
	while (1) {
		sem_post(&sem);
		PUTCHARF('-');
		poll(NULL, 0, 100);
	}
}

void
alarm_handler(int s)
{
	alarm(1);
}

int
main(int argc, char *argv[])
{
	pthread_t t;
	int nointr = 0;
	struct sigaction act;

	if (argc == 2)
		nointr = 1;

	/* Allow to enter 5 times. */
	if (sem_init(&sem, 0, 5) == -1)
		err(1, "sem_init");

	pthread_create(&t, NULL, usher, NULL);

	act.sa_handler = alarm_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, NULL);
	alarm(1);

	while (1) {
		if (nointr) {
			sem_wait_nointr(&sem);
		} else {
			if (sem_wait(&sem) != 0)
				err(1, "sem_wait");
		}
		PUTCHARF('_');
		/* Run more frequently than usher. */
		poll(NULL, 0, 10);
	}

	sem_destroy(&sem);

	return (0);
}
