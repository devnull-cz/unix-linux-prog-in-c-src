/*
 * Run the program with and without argument and compare the process trees.
 */

#define	_XOPEN_SOURCE	700

#include <err.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	char c;
	int pid;
	int ignore = 0;

	if (argc > 1) {
		printf("will ignore SIGCHLD\n");
		ignore = 1;
	}

	switch (pid = fork()) {
	case -1:
		err(1, "fork");
	case 0:
		printf("%d: this is a child\n", getpid());
		pause();
		break;
	default:
		printf("%d: this is a parent\n", getpid());
		printf("%d: type anything (only the 1st char matters)...\n",
		    getpid());
		if (read(0, &c, 1) != 1)
			err(1, "read");

		if (ignore) {
			struct sigaction s_action;
			(void) sigemptyset(&s_action.sa_mask);
			s_action.sa_handler = SIG_IGN;
			s_action.sa_flags = 0;
			(void) sigaction(SIGCHLD, &s_action, NULL);
		}

		printf("%d: sending a signal to %d\n", getpid(), pid);
		kill(pid, SIGTERM);
		// wait(NULL);
		printf("%d: waiting for two signals\n", getpid());
		pause();
		printf("got one\n");
		pause();
	}
}
