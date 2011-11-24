#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(void)
{
	int pid;
	int pd[2];

	pipe(pd);

	switch (pid = fork()) {
	case -1:
		fprintf(stderr, "error\n");
		exit(1);
	case 0:
		close(1);
		dup(pd[1]);
		// not before dup() !
		close(pd[0]);
		close(pd[1]);
		execl("/bin/ls", "/bin/ls", "/", (char *) NULL);
		break;
	default:
		close(0);
		dup(pd[0]);
		close(pd[0]);
		close(pd[1]);
		execl("/usr/bin/less", "/usr/bin/less", (char *) NULL);
		break;
	}

	return (0);
}
