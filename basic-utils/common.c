#include <stdio.h>
#include <unistd.h>

void function_x(void)
{
	printf("hello world\n");
	/* see preload.c */
	close(2);
}
