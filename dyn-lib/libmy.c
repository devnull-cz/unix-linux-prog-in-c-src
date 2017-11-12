/*
 * This is not standalone code.  See the block comment in dlopen.c (should be in
 * the same directory as this file) on how to use it.
 */
#include <stdio.h>

extern void foo(void);

void
myprint(void)
{
	printf("hello\n");
	foo();
}
