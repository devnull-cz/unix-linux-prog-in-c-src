/*
 * An example on how a change in a lib ABI can screw up programs that use the
 * library.
 *
 * The program adds 2 32-bit integers. When lib's ABI changes, the program
 * doesn't know about it and prints an incorrect value. Run like this:
 *
 *   $ gcc -m32 -shared -o libabi.so libabi-32.c
 *   $ gcc -m32 -L. -Xlinker -R . -labi abi-main.c
 *   $ ./a.out 7 4
 *   10002000
 *   result: 11
 *   $ gcc -m32 -shared -o libabi.so libabi-64.c
 *   $ ./a.out 7 4
 *   10002000
 *   result: 2007
 *
 * For Sun Studio (cc), use "-G" instead of "-shared". Also note that newer
 * GCC does not support "-R" so we have to use "-Xlinker -R ." instead
 * (note the ``.'' as part of the option).
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

extern int32_t abiadd(int32_t a, int32_t b);

int
main(int argc, char **argv)
{
	if (argc != 3)
		errx(1, "usage: a.out <number> <number>");

	/* Put some data on the stack first by calling printf(). */
	printf("%d%d\n", 1000, 2000);

	/* See the result. */
	printf("result: %d\n", abiadd(atoi(argv[1]), atoi(argv[2])));

	return (0);
}
