/*
 * Note that when fork(2) is used from a thread that is not main() then such
 * a thread can live in the child process as long as it wants since there is no
 * main() thread to exit - the main function from then on is the thread function
 * itself now.
 *
 * (by) jp@devnull.cz
 */

#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>

void
*thread(void *x)
{
	int i;
	pid_t pid;

	printf("%d: before fork()\n", getpid());
	pid = fork();

	if (pid == 0)
		printf("%d: I'm child\n", getpid());
	else
		printf("%d: I'm parent\n", getpid());

	for (i = 0; i < 20; ++i) {
		printf("%d: loop #%d\n", getpid(), i);
		sleep(1);
	}
	return (NULL);
}

int
main(void)
{
	pthread_t t;

	pthread_create(&t, NULL, thread, NULL);
	sleep(1);
	printf("%d in main: exiting.\n", getpid());

	return (0);
}
