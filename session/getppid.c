/*
 * Create child process and make it wait for its parent to exit and then
 * to report PID of its new (adoptive) parent. In most cases it will be 1
 * and belong to the init process but there are other possibilities - see
 * the note in the code below.
 *
 * (c) vlada@devnull.cz, 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

int
main(void)
{
	pid_t child_pid, parent_pid;
	int status;

	switch (child_pid = fork()) {
		case -1:
			fprintf(stderr, "fork failed\n");
			exit(1);
		case 0:
			parent_pid = getppid();
			printf("child: pid = %d parent = %d\n",
			    getpid(), parent_pid);

			printf("child: waiting for parent to exit\n");
			/*
			 * We cannot use waitpid(parent_pid, &status, 0);
			 * since it only works on child processes.
			 * So use null signal to check if the process is
			 * still alive.
			 */
			while (kill(parent_pid, 0) != -1) {
				printf("child: sleeping\n");
				sleep(1);
			}
			if (errno == ESRCH) {
				printf("child: parent has exited, current "
				    "parent: %d\n", getppid());
				/*
				 * NOTE: we could check if current PID is 1
				 *	 to see if init has taken over the
				 *	 parenthood but this is not universally
				 *	 useable, e.g. in Solaris zone, the
				 *	 parent would be 'zsched' process
				 *	 with PID different to 1 (in fact init
				 *	 process in a zone has PID != 1).
				 */
				exit(0);
			}
			break;
		default:
			printf("parent: pid = %d child pid: %d "
			    "grand parent = %d\n", getpid(), child_pid,
			    getppid());
			sleep(2);
			printf("parent: exiting\n");
			exit(0);

			/* NOT REACHED */
	}

	return (0);
}
