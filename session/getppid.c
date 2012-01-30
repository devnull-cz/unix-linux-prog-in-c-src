#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	printf("parent pid is: %d\n", getppid());
	return (0);
}
