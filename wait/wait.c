/*
 * Example program to see how waitpid(3) works. Start the program. After that,
 * you can do 4 different things:
 *
 *   (1) wait quietly for 30 seconds to see the parent report sleep's return
 *   value which should be 0
 *   (2) kill sleep (use the child's PID printed upon startup) to see the signal
 *   number used. Try different signals (kill <PID>, kill -9 <PID>, ...)
 *   (3) stop the child to see that the parent gets notified about that. You can
 *   stop a process by sending it a stop signal: kill -STOP <PID>. Again, use
 *   the child's PID.
 *   (4) continue the child by "kill -CONT <PID>"
 *
 * The parent will continue to report on the child states until the child is
 * finished. Note that if the sleep is stopped it won't realize the 30 seconds
 * timeout unless it gets continued again.
 *
 * Note that when the child is killed by a signal, the status word itself
 * may contain just the signal number (tested on OpenSolaris). However, this is
 * just a coincidence and can be implementation dependent! Always use the
 * WIFSIGNALED macro to get the signal number.
 *
 * If WUNTRACED was not used in the flags parameter, the parent wouldn't be
 * notified about the stopped children. Try to remove it and then "kill -STOP"
 * the child to see that. The same stands for continuing the child - the parent
 * wouldn't get notified about continued children unless WCONTINUED is present.
 * Also note that stopping an already stopped process is not reported, the same
 * stands for continued processes. See waitpid(3) for more information.
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

int
main(void)
{
	int pid, status;

	switch (pid = fork()) {
	case -1:
		fprintf(stderr, "error\n");
		exit(1);
	case 0:
		printf("child's PID: %d\n", (int)getpid());
		printf("child exec'ing 'sleep 30'\n");
		execl("/bin/sleep", "sleep", "30", NULL);

		printf("execl() error: %s\n", strerror(errno));
		exit(1);

		break;
	default:
		printf("parent's PID: %d\n", (int)getpid());

		/* let's loop until the child is finished */
		while (1) {
			waitpid(pid, &status, WUNTRACED | WCONTINUED);

			printf("status integer in parent is: %d\n", status);

			if (WIFEXITED(status)) {
				printf("child exited with: %d\n",
				    WEXITSTATUS(status));
				break;
			}

			if (WIFSIGNALED(status)) {
				printf("child signalled, killed by signal %d\n",
				    WTERMSIG(status));
				break;
			}

			if (WIFSTOPPED(status)) {
				printf("child stopped\n");
			}

			if (WIFCONTINUED(status)) {
				printf("child continued\n");
			}
		}
	}

	return (0);
}
