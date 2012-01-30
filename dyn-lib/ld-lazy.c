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
 * Lazy binding: cc -z lazyload -lm ld-lazy.c
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
 * (c) jp@devnull.cz
 */
#include <sys/wait.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	char buf[16];

	/*
	 * We don't want to actually call it so that we can see the difference
	 * in lazy binding.
	 */
	if (0) {
		/* libm */
		(void) sin(0);
	}

	snprintf(buf, sizeof (buf), "%d", getpid());

	/* we must fork now to pldd our process */
	if (fork() == 0) {
		execl("/usr/bin/pldd", "pldd", buf, NULL);
		fprintf(stderr, "execl() failed on pldd: %s\n",
		    strerror(errno));
		fprintf(stderr, "are you sure this is Solaris?\n");
		exit(1);
	} else {
		/* be sure we don't die before we are pldd'ed */
		wait(NULL);
	}

	return (0);
}
