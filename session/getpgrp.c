#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	fprintf(stderr, "process group id: %d\n", getpgrp());
	fprintf(stderr, "my pid is: %d\n", getpid());

	return (0);
}
