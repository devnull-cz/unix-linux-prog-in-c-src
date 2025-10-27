/*
 * Simple posix_spawn() demonstration.
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

static void
spawnit(char *progname)
{
	int error, status;
	pid_t pid;
	char *argv[3] = { "ps", "-j", NULL };

	if ((error = posix_spawn(&pid, progname, NULL, NULL, argv,
	    NULL)) != 0) {
		warn("posix_spawn for %s", progname);
		return;
	}

	/*
	 * Error detection is not easy with posix_spawn(). It can happen that
	 * the child process fails to execute the program after posix_spawn()
	 * returned success.
	 *
	 * NOTE: Checking with WNOHANG is not reliable as this is scheduling
	 *	 dependent !
	 */
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 127) {
		fprintf(stderr, "posix_spawn failed for %s\n", progname);
		return;
	}

	printf("pid = %d\n", pid);
}

int
main(void)
{
	spawnit("/bin/foo");
	spawnit("/bin/ps");

	return (0);
}
