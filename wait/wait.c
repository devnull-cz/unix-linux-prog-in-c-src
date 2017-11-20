/*
 * An example program to see how waitpid(3) works.  Start the program.  After
 * that, you can do 4 different things:
 *
 *   (1) wait quietly for 30 seconds to see the parent report sleep's return
 *       value which should be 0
 *   (2) kill sleep (use the child's PID printed upon startup) to see the
 *       signal number used.  Try different signals (kill <PID>, kill -9 <PID>,
 *       ...)
 *   (3) stop the child to see that the parent gets notified about that. You
 *       can stop a process by sending it a stop signal: kill -STOP <PID>.
 *       Again, use the child's PID.
 *   (4) continue the child by "kill -CONT <PID>"
 *
 * The parent will continue to report on the child states until the child is
 * finished.  Note that if the sleep is stopped it won't realize the 30 seconds
 * timeout unless it gets continued again.
 *
 * Note that when the child is killed by a signal, the status word itself
 * may contain just the signal number.  However, that is just a coincidence and
 * can be implementation dependent!  Always use the WIFSIGNALED and WTERMSIG
 * macros to get the signal number.
 *
 * If WUNTRACED was not used in the flags parameter, the parent would not be
 * notified about stopped children.  Try to remove it and then "kill -STOP"
 * the child to see that.  The same stands for continuing the child - the parent
 * would not get notified about continued children unless WCONTINUED is present.
 * Also note that stopping an already stopped process is not reported, the same
 * stands for continued processes.
 *
 * See waitpid(3) for more information.
 *
 * (c) jp@devnull.cz
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <assert.h>

int
main(void)
{
	pid_t pid;
	int status;

	if ((pid = fork()) == -1)
		err(1, "fork");

	if (pid == 0) {
		printf("Child's PID: %d\n", (int)getpid());
		printf("Child exec'ing 'sleep 30'\n");
		execl("/bin/sleep", "sleep", "30", NULL);
		err(1, "execl");
	}

	printf("parent's PID: %d\n", getpid());

	/* Let's loop until the child is finished. */
	while (1) {
		pid_t pid2;

		pid2 = waitpid(pid, &status, WUNTRACED | WCONTINUED);
		assert(pid == pid2);

		if (WIFEXITED(status)) {
			printf("Child called exit with: %d\n",
			    WEXITSTATUS(status));
			break;
		}

		if (WIFSIGNALED(status)) {
			printf("Child killed by signal %d.\n",
			    WTERMSIG(status));
			break;
		}

		if (WIFSTOPPED(status))
			printf("Child stopped.\n");
		if (WIFCONTINUED(status))
			printf("Child continued.\n");
	}

	return (0);
}
