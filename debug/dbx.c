/*
 * A simple program to run with dbx(1).
 */

#include <stdio.h>
#include <unistd.h>

int helper(void)
{
	int i = 17;
	return (i);
}

int
main(void)
{
	int i = 13;
	int *pi = (int *)0x01;

	printf("i is %d\n", i);
	i = helper();
	sleep(1);
	*pi = 1;
	return (0);
}
