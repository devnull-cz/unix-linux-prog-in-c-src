/*
 * This should show the example of MAP_NOCORE use. Compile the program, run it
 * on the background, gcore(1) it, and use strings(1) with "-a" on it to grep
 * "my-PIN". It should be there for the 1st case and it should be clean for the
 * 2nd case.
 *
 * FreeBSD supports the flag, Solaris does not. Unfortunately, I can see that on
 * FreeBSD 6.0 the PIN is still in the core.
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/mman.h>

int
main(void)
{
	char *addr = NULL;

#if 1
	addr = mmap(0, 4096, PROT_READ | PROT_WRITE,
	    MAP_SHARED | MAP_ANON, -1, 0);
#else
	addr = mmap(0, 4096, PROT_READ | PROT_WRITE,
	    MAP_SHARED | MAP_ANON | MAP_NOCORE, -1, 0);
#endif

	if ((void *)addr == MAP_FAILED)
		err(1, "mmap");

	/*
	 * The password is "my-PIN". We put the PIN there the way we use because
	 * if we used a static string it would end up in a data segment.
	 */
	addr[0] = 'm';
	addr[1] = 'y';
	addr[2] = '-';
	addr[3] = 'P';
	addr[4] = 'I';
	addr[5] = 'N';
	addr[6] = '\0';

	sleep (99);

	return (0);
}
