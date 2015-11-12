/*
 * The simplest fork() example. You should see how the parent and the child
 * are printing messages in turns. Parent exits first, child continues along
 * and its parent is now the init process (which *usually* has pid = 1).
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(void)
{
	int i, pid, limit;
	char *name;

	switch (pid = fork()) {
	case -1:
		fprintf(stderr, "error\n");
		exit(1);
	case 0:
		printf("I'm child, my PID is %d\n", getpid());
		name = "child";
		limit = 8;
		break;
	default:
		printf("I'm parent, PID is %d\n", getpid());
		name = "parent";
		limit = 3;
		break;
	}

	for (i = 0; i < limit; ++i) {
		printf("%s: loop %d [parent %d]\n", name, i, getppid());
		sleep(1);
	}

	return (0);
}
