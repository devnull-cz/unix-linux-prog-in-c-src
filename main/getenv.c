/*
 * A simple example on getenv() function.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>

int
main(void)
{
	printf("USER=%s\n", getenv("USER"));

	printf("now setting variable X to 'yyy'\n");
	putenv("X=yyy");
	printf("and running env(1) to check whether it's set in a child...\n");

	if (fork() == 0) {
		printf("\n[/usr/bin/env | grep ^X=]\n");
		/* child inherited X */
		system("/usr/bin/env | grep ^X=");
	}

	/* wait for my child */
	wait(-1);
	return (0);
}
