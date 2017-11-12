/*
 * Check if process with a given PID exists.
 *
 * (c) Vladimir Kotal, 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	pid_t pid;
	
	if (argc != 2)
		errx(1, "Need a PID as argument.");
		
	pid = atoi(argv[1]);
	if ((kill(pid, 0) == 0) || (errno == EPERM))
		printf("process with pid %d exists\n", pid);
	else
		err(1, "kill for process with pid %d failed", pid);

	return (0);
}
