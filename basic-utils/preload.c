#include <unistd.h>
#include <stdio.h>

int close(int d)
{
	(void) printf("fd %d will not be closed!\n", d);
	return (0);
}
