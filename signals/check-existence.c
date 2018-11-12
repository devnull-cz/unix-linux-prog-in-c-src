/*
 * Check if process with a given PID exists.
 *
 * (c) Vladimir Kotal, 2016
 */

#define	_XOPEN_SOURCE	700

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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
	if ((kill(pid, 0) == -1) && (errno != EPERM))
		err(1, "Failed signalling pid %d.", pid);

	printf("Process with pid %d exists.\n", pid);

	return (0);
}
