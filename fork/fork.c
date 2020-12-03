/*
 * The simplest fork() example. You should see how the parent and the child
 * are printing messages in turns. The parent exits first, its child carries on
 * and the init process is assigned as its parent instead (which *usually* has
 * pid = 1).
 *
 * Note that you get a new prompt before the child finishes as the shell waits
 * only for the command it started, not for its children.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void)
{
	int i, pid, limit;
	char *name;

	switch (pid = fork()) {
	case -1:
		err(1, "fork");
	case 0:
		printf("I'm child, my PID is %d\n", getpid());
		name = "child";
		limit = 8;
		break;
	default:
		printf("I'm parent, PID is %d, my child is %d\n", getpid(), pid);
		name = "parent";
		limit = 3;
		break;
	}

	for (i = 0; i < limit; ++i) {
		printf("%s: loop %d [my parent is %d]\n", name, i, getppid());
		sleep(1);
	}
}
