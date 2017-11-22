/*
 * Create a child process and make it wait for its parent to exit, then report
 * the PID of its new (adoptive) parent.  It will be 1 in most cases, meaning
 * the init process will adopt the child but there are other possibilities - see
 * the note in the code below.
 *
 * (c) vlada@devnull.cz, 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>

int
main(void)
{
	pid_t child_pid, parent_pid;

	if ((child_pid = fork()) == -1)
		err(1, "fork");

	if (child_pid != 0) {
		printf("parent: pid = %d child pid: %d grandparent = %d\n",
		    getpid(), child_pid, getppid());
		sleep(2);
		printf("parent: exiting\n");
		exit(0);
	}

	assert(child_pid == 0);
	parent_pid = getppid();
	printf("child: pid = %d parent = %d\n", getpid(), parent_pid);
	printf("child: waiting for parent to exit\n");

	/*
	 * We cannot use waitpid(parent_pid, &status, 0); since it only works on
	 * child processes.  So use the 0 signal to check if the process is
	 * still alive.
	 */
	while (kill(parent_pid, 0) != -1) {
		printf("child: sleeping\n");
		sleep(1);
	}
	if (errno == ESRCH) {
		printf("child: parent exited, current parent: %d\n", getppid());
		/*
		 * NOTE: we could check if the current PID is 1 to see if init
		 * has taken over the parenthood but this is not universally
		 * useable, e.g. in a Solaris zone the parent would be 'zsched'
		 * process with a PID different to 1 (in fact the init process
		 * in a zone has PID != 1).
		 */
		exit(0);
	}

	return (0);
}
