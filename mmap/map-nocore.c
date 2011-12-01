/*
 * Example on MAP_NOCORE use. Build it, run it on the background, dump it (eg.,
 * "kill -3 <PID"), and use strings(1) with "-a" on it to grep "my-PIN". It
 * should be there for the 1st case and it should be clean for the 2nd case.
 *
 * FreeBSD supports the flag, Solaris does not. Note that gcore(1) would write
 * the whole process to a file, including the NOCORE segment. That's quite
 * logical since this operation can be done by somebody who can examine the
 * process memory anyway. The objective is that the mmaped page is not in the
 * core dump caused by the kernel (on SIGSEGV etc.).
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/mman.h>

int
main(int argc, char *argv[])
{
	char *addr = NULL;
	int flags = MAP_SHARED | MAP_ANON;

	if (argc != 2)
		errx(1, "usage: %s <0|1>", argv[0]);

	if (atoi(argv[1]) == 1)
#if defined(__FreeBSD__)
		flags |= MAP_NOCORE;
#else
		errx(1, "cannot perform the operation, no MAP_NOCORE support");
#endif

	addr = mmap(0, 4096, PROT_READ | PROT_WRITE, flags, -1, 0);

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
