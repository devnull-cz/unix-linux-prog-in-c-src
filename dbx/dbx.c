#include <stdio.h>
#include <unistd.h>

int
helper(void)
{
	int i = 17;

	return (i);
}

void
crash(int *p)
{
	*p = 1;
}

int
main(void)
{
	int i = 13;
	int *p = (int *) 0x01;

	printf("i is %d\n", i);
	i = helper();
	sleep(1);
	crash(p);

	return (0);
}
