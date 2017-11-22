/*
 * Demonstrate what is sigqueue() good for.
 *
 * compile and run:
 *   cc sigqueue.c
 *   ./a.out
 *   echo $?
 *
 * (c) vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef _POSIX_REALTIME_SIGNALS
#error	"sorry, POSIX.4 real time signals extensions missing."
#endif

volatile sig_atomic_t num;

void
handler(int sig, siginfo_t *info, void *foo)
{
	if (info != NULL)
		num = info->si_value.sival_int;
}

int
main(void)
{
	struct sigaction act = { 0 };
	union sigval val;
	pid_t pid;
	int status;

	switch (pid = fork()) {
		case -1:
			perror("fork");
			break;
		case 0:
			/*
			 * We have to use SIGINFO extension to be able to
			 * pass a value to signal handler.
			 */
			act.sa_sigaction = handler;
			act.sa_flags = SA_SIGINFO;
			sigaction(SIGINT, &act, NULL);

			printf("waiting\n");
			pause();
			printf("exiting %d\n", num);
			exit(num);
			break;
		default:
			/*
			 * We should block SIGINT in the parent and unblock
			 * it in the child only after the handler is set,
			 * otherwise the signal will terminate the child
			 * before it can setup the handler.
			 * Use sleep as a crude way to avoid this.
			 */
			printf("%d sleeping\n", getpid());
			sleep(1);

			val.sival_int = 42;
			printf("%d sending\n", pid);
			sigqueue(pid, SIGINT, val);
			waitpid(pid, &status, 0);
			exit(WEXITSTATUS(status));
			break;
	}

	return (0);
}
