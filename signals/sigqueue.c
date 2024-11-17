/*
 * Demonstrate what is sigqueue() and how it can be used to pass integers
 * between processes.
 *
 * Compile and run:
 *
 *	$ cc sigqueue.c
 *	$ ./a.out 98
 *	367968 sleeping
 *	367969 waiting
 *	367968 sending SIGINT signal to 367969
 *	367969 exiting with received value of 98
 *	$ echo $?
 *	99
 */

#define	_XOPEN_SOURCE	700

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef _POSIX_REALTIME_SIGNALS
#error	"Sorry, POSIX.4 real time signals extensions missing."
#endif

volatile sig_atomic_t num;

void
handler(int sig, siginfo_t *info, void *foo)
{
	if (info != NULL)
		num = info->si_value.sival_int;
}

int
main(int argc, char **argv)
{
	struct sigaction act = { 0 };
	union sigval val;
	pid_t pid;
	int status;

	if (argc != 2)
		errx(1, "One small numerical argument required");

	if ((pid = fork()) == -1)
		err(1, "fork");

	if (pid == 0) {
		/*
		 * We have to use the SIGINFO extension to be able to pass a
		 * value to a signal handler.
		 */
		act.sa_sigaction = handler;
		act.sa_flags = SA_SIGINFO;
		sigaction(SIGINT, &act, NULL);

		printf("%d waiting\n", getpid());
		pause();
		printf("%d exiting with received value of %d\n", getpid(), num);
		exit(num + 1);
	}

	/*
	 * We should block SIGINT in the parent and unblock it in the
	 * child only after the handler is set otherwise the signal will
	 * terminate the child before it can setup the handler.
	 * Use sleep as a crude way to avoid this.
	 */
	printf("%d sleeping 1 second\n", getpid());
	sleep(1);

	val.sival_int = atoi(argv[1]);
	printf("%d sending SIGINT signal to %d\n", getpid(), pid);
	sigqueue(pid, SIGINT, val);
	waitpid(pid, &status, 0);
	exit(WEXITSTATUS(status));
}
