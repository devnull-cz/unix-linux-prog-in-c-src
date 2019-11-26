/*
 * The SIGCHLD signal is always delivered to the parent process except
 * in case it is ignored.
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void
handler(int s)
{
	write(1, "got it\n", 7);
}

int
main(int argc, char *argv[])
{
	pid_t pid;

#ifdef HANDLE_SIGCHLD
	struct sigaction act;
	// act.sa_handler = handler;
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, NULL);
#endif

	if ((pid = fork()) == 0) {
		sleep(1);
	} else {
		int status;
		printf("forked %d\n", pid);
		// wait(&status);
		waitpid(pid, &status, 0);
	}

	return (0);
}
