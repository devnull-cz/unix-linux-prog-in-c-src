/*
 * Will crash and may generate a core dump.
 *
 * You might want to run "ulimit -c size" with reasonable non-zero size to
 * actually get the core file.  The easiest might be "ulimit -c unlimited" but
 * the size of 1000 (0.5MB) should be enough for this.
 */
#include <stdio.h>

void
bad_memory_access(void)
{
	char *x = NULL;

	x[0] = 'A';
}

int
main(void)
{
	bad_memory_access();
	/* NOTREACHED */
}
