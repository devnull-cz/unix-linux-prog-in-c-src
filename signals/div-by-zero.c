/*
 * Divide by zero example.  Use something that will not be optimized out or
 * warned about by the compiler.
 */
#include <stdio.h>

int
main(void)
{
	int i = 0;

	printf("%d\n", 999 / i);
}
