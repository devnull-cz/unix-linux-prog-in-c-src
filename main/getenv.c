/*
 * A simple example on getenv() function.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(void)
{
	printf("Printing USER env variable:\n");
	printf("USER=%s\n", getenv("USER"));

	printf("Now setting variable X to 'yyy'.\n");
	putenv("X=yyy");
	printf("And running env(1) to check whether it's set in a child...\n");

	if (fork() == 0) {
		printf("\n[/usr/bin/env | grep X=]\n");
		/* Child inherited X. */
		system("/usr/bin/env | /bin/grep ^X=");
	}

	/* wait for my child */
	wait(NULL);
	return (0);
}
