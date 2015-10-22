/*
 * Try compiling with NDEBUG defined and without.
 *
 * cc assert.c
 * ./a.out
 * cc -DNDEBUG assert.c
 * ./a.out
 *
 * and see the difference as described in assert(3) man page.
 */

#include <assert.h>

int
main(void)
{
	assert(1 == 0);
	return (13);
}
