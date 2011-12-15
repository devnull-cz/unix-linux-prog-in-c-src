#include <stdio.h>

extern void foo(void);

void myprint(void)
{
	printf("hello\n");
	foo();
}
