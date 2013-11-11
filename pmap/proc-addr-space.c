/*
 * A sample program to see where different parts of memory fit into. You should
 * know what is the exact process address space layout on your system and
 * architecture. You can use pmap(1) on Linux or Solaris to see the segments
 * of the running process.
 *
 * Read README in this directory for some example outputs.
 *
 * Run like this:
 *   gcc proc-addr-space.c
 *   ./a.out
 *   ./a.out 16
 *   ./a.out 1048576
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int nonini1;
int ini1 = 3;
int ini2 = 3;

int
main(int argc, char **argv)
{
	void *p;
	char buf[16];
	int to_alloc = 0;

	if (argc > 1)
		to_alloc = atoi(argv[1]);

	if (to_alloc > 0) {
		if ((p = malloc(to_alloc)) == NULL) {
			fprintf(stderr, "malloc failed: %s\n", strerror(errno));
			return (1);
		}
	}

	printf("addresses:\n");
	if (to_alloc > 0)
		printf("  variable on the heap:\t%p\n", p);
	printf("  local variable:\t%p\n", buf);
	printf("  noninitialized var:\t%p\n", &nonini1);
	printf("  initialized var 1:\t%p\n", &ini1);
	printf("  initialized var 2:\t%p\n", &ini2);

	printf("\n=== pmap %d ===\n", getpid());
	snprintf(buf, sizeof (buf), "%d", getpid());

	/* we must fork so that don't overwrite those variables above */
	if (fork() == 0) {
		execl("/usr/bin/pmap", "pmap", buf, NULL);
		fprintf(stderr, "execl() failed on pmap: %s\n",
		    strerror(errno));
		return (1);
	}

	/* be sure we don't die before we are pmap'ed */
	wait(NULL);
	return (0);
}
