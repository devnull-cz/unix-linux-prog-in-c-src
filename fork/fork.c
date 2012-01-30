/*
 * The simplest fork() example. You should see how the parent and the child
 * are printing messages in turns.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(void)
{
	int i, pid;
	char *name;

	switch (pid = fork()) {
	case -1:
		fprintf(stderr, "error\n");
		exit(1);
	case 0:
		printf("I'm child, PID is %d\n", getpid());
		name = "child";
		break;
	default:
		printf("I'm parent, PID is %d\n", getpid());
		name = "parent";
		break;
	}

	for (i = 0; i < 6; ++i) {
		printf("%s: loop %d\n", name, i);
		sleep(1);
	}

	return (0);
}
