/*
 * Compile this program twice (and run it then) to see a difference with lazy
 * binding. Tested on Solaris with Sun Studio compiler.
 *
 * Normal way: cc -lm ld-lazy.c
 *
 *  $ ./a.out
 *  7273:   ./a.out
 *  /lib/libm.so.2
 *  /usr/lib/libc/libc_hwcap1.so.1
 *
 * Lazy binding:
 *   Solaris:
 *     cc -z lazyload -lm ld-lazy.c
 *   Linux:
 *     gcc -z lazy -lm ld-lazy.c
 *
 * Solaris:
 *
 *  $ ./a.out
 *  7273:   ./a.out
 *  /usr/lib/libc/libc_hwcap1.so.1
 *
 * The reason why libc is there is that it was already used by the program
 * with fork(), for example. The difference in ELF when lazy loading is
 * requested is the LAZY flag:
 *
 *   $ elfdump -d a.out
 *   ...
 *   [0]  POSFLAG_1         0x1                 [ LAZY ]
 *   [1]  NEEDED            0x187               libm.so.2
 *   [2]  NEEDED            0x19a               libc.so.1
 *
 * And you can also use LD_BIND_NOW environment variable to force non-lazy
 * binding even when "-z lazyload" was used: LD_BIND_NOW=yes ./a.out
 *
 * Linux:
 *
 *   $ readelf -d ./a.out
 *   Dynamic section at offset 0xf04 contains 26 entries:
 *     Tag        Type                         Name/Value
 *    0x00000001 (NEEDED)                     Shared library: [libm.so.6]
 *    0x00000001 (NEEDED)                     Shared library: [libc.so.6]
 *
 * WARNING: as of Nov 2017, pldd(1) on Linux is broken.  See the man page.
 *
 * (c) jp@devnull.cz
 */

#if !defined(__sun) && !defined(__SVR4) && !defined(__linux__)
#error This code may only run on Solaris and Linux.
#endif

#include <sys/wait.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	pid_t pid;
	char buf[16];

	/*
	 * We do not want to actually call it so that we can see the difference
	 * in lazy binding.  However, we must reference something from libm.so
	 * in the code.
	 */
	if (0) {
		/* libm */
		(void) sin(0);
	}

	snprintf(buf, sizeof (buf), "%d", getpid());
	/* We must fork now to pldd our process. */
	if ((pid = fork() == 0)) {
		execl("/usr/bin/pldd", "pldd", buf, NULL);
		err(1, "execl() failed on pldd");
	} else {
		if (pid == -1)
			err(1, "fork");
		/* Be sure we do not die before we are pldd'ed. */
		wait(NULL);
	}

	return (0);
}
