/*
 * Example on MAP_NOCORE use. On FreeBSD, build it, run it and wait for the core
 * dump.  Then use strings(1) with "-a" on the core to grep "my-PIN".  It should
 * be there for argv1 == 0 and it should be clean for argv1 == 1.
 *
 * FreeBSD supports the flag but neither Linux, Solaris nor macOS do.  Note that
 * gcore(1) on Solaris would write the whole process to a file, including the
 * NOCORE segment.  That's quite logical since this operation can be done by
 * somebody who can examine the process memory anyway.  The objective is that
 * the mmap'ed page is not in the core dump caused by the kernel (on SIGSEGV
 * etc.).
 */

#define	_DEFAULT_SOURCE	// to get MAP_ANONYMOUS on Linux

#include <err.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	char *addr = NULL;
	int flags = MAP_SHARED | MAP_ANONYMOUS;

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
	 * The password is "my-PIN".  We put the PIN there like that because if
	 * we used a static string it would end up in the .data segment.  Now it
	 * ends up in .text segment.  Anyway, for any real world use it would
	 * come from external source (pipe, stdin, ...).
	 */
	addr[0] = 'm';
	addr[1] = 'y';
	addr[2] = '-';
	addr[3] = 'P';
	addr[4] = 'I';
	addr[5] = 'N';
	addr[6] = '\0';

	kill(getpid(), SIGQUIT);
}
