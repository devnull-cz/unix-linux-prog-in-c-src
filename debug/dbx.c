/*
 * Compile as:
 *   - Linux:   gcc dbx.c
 *   - Solaris: cc dbx.c
 *
 * Run as:
 *   - Linux:   ./a.out; gdb ./a.out core
 *   - Solaris: ./a.out; dbx ./a.out core
 *
 * In the debugger type 'where' and see the difference.
 *
 * Use the -g option when compiling with gcc and try again.
 *
 * You might want to run 'ulimit -c size' with reasonable non-zero size
 * before running the program to actually get the core file.
 */

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
