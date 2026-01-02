/*
 * Demonstrate how pthread_atfork() works.  The before() handler is called from
 * the thread which called fork().  While the before() handler is running, all
 * the other threads are still running as well - that is important since they
 * need to eventually release the locks that the before() handler is waiting
 * for.
 *
 * Note that in order to see which threads call the handlers we are using an
 * array of pthread_t values associated with the thread name.  The other
 * alternative would be to use a thread specific variable.
 *
 * Note that the "counter" thread does not call any of the atfork handlers.
 *
 * To see what happens if we fork without acquiring the mutex via the pre-fork
 * handler, use the -d switch.  The program will hang indefinitely as the parent
 * waits for a child that never exits.
 *
 * 	$ ./atfork -d
 *
 * vlada@devnull.cz, jp@devnull.cz
 */

#include <err.h>
#include <libgen.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define	NLOOPS	3
#define	USAGESTR	"usage: %s [-d] [<nsecs>]"

pid_t pid;
int atfork = 1;
int nsecs = 5;

typedef struct {
	char		name[16];
	pthread_t	t;
} thread_info_t;

enum {
	FORKER = 0,
	COUNTER = 1,
	MAX_ID = 2,
} thread_idx;

pthread_mutex_t mutex;
thread_info_t threads[MAX_ID];

/*
 * This is a scheme to deal with thread IDs in a transparent way - in general we
 * cannot rely on thread IDs to be of a known type (e.g. integers).
 */
static char *
get_id(void)
{
	for (unsigned int i = 0; i < MAX_ID; i++) {
		if (pthread_equal(pthread_self(), threads[i].t) != 0)
			return (threads[i].name);
	}

	return (NULL);
}

void *
forker(void *x)
{
	threads[FORKER].t = pthread_self();
	strncpy(threads[FORKER].name, "forker", sizeof (threads[FORKER].name));

	printf("%d/%s: forking a process\n", getpid(), get_id());
	if ((pid = fork()) == -1)
		err(1, "fork");
	if (pid == 0)
		printf("%d: I'm child\n", getpid());
	else
		printf("%d: I'm parent\n", getpid());

	/*
	 * By correctly using the pthread_atfork(), it is guaranteed that we
	 * will be able to lock the mutex now.
	 */
	printf("%d/%s: locking mutex\n", getpid(), get_id());
	if (pthread_mutex_lock(&mutex) != 0)
		printf("mutex lock failed\n");
	printf("%d/%s: mutex locked\n", getpid(), get_id());

	printf("%d/%s: releasing mutex\n", getpid(), get_id());
	if (pthread_mutex_unlock(&mutex) != 0)
		printf("mutex unlock failed\n");

	return (NULL);
}

void *
counter(void *arg)
{
	threads[COUNTER].t = pthread_self();
	strncpy(threads[COUNTER].name, "counter",
	    sizeof (threads[COUNTER].name));

	while (1) {
		printf("%d/%s: just counting seconds...\n", getpid(),
		    get_id());
		sleep(1);
	}

	return (NULL);
}

/*
 * Capitalize the handler name to make it stand out in the output.
 */
void
before(void)
{
	printf("%d/%s: in the \"BEFORE\" handler, grabbing the mutex...\n",
	    getpid(), get_id());
	if (pthread_mutex_lock(&mutex) != 0)
		printf("mutex lock failed\n");
	printf("%d/%s: leaving the \"BEFORE\" handler holding the mutex\n",
	    getpid(), get_id());
}

/*
 * Capitalize the handler name to make it stand out in the output.
 */
void
after(void)
{
	printf("%d/%s: in the \"AFTER\" handler, releasing the mutex\n",
	    getpid(), get_id());
	if (pthread_mutex_unlock(&mutex) != 0)
		printf("mutex unlock failed\n");
	printf("%d/%s: leaving the \"AFTER\" handler with the mutex released\n",
	    getpid(), get_id());
}

int
main(int argc, char **argv)
{
	pthread_t forker_thr, counter_thr;

	/* When you don't want to use getopt()... */
	if (argc > 1) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'd')
				atfork = 0;
			else
				errx(1, USAGESTR, basename(argv[0]));
			if (argc == 3)
				nsecs = atoi(argv[2]);
			else if (argc > 3)
				errx(1, USAGESTR, basename(argv[0]));
		} else {
			if (argc == 2)
				nsecs = atoi(argv[1]);
			else
				errx(1, USAGESTR, basename(argv[0]));
		}
	}

	/*
	 * pthread_atfork() should be called after locks are initialized
	 * and before they are used.  Libraries which do this in .init section
	 * should use pthread_once().
	 */
	(void) pthread_mutex_init(&mutex, NULL);
	if (atfork)
		(void) pthread_atfork(before, after, after);

	printf("%d in main: locking mutex\n", getpid());
	if (pthread_mutex_lock(&mutex) != 0)
		printf("mutex lock failed\n");

	(void) pthread_create(&forker_thr, NULL, forker, NULL);
	(void) pthread_create(&counter_thr, NULL, counter, NULL);

	printf("%d in main: sleeping for %d seconds, holding the mutex\n",
	    getpid(), nsecs);
	sleep(nsecs);

	printf("%d in main: unlocking mutex\n", getpid());
	if (pthread_mutex_unlock(&mutex) != 0)
		printf("mutex unlock failed\n");
	printf("%d in main: waiting for the threads\n", getpid());

	/* Wait for the "forker" thread to complete. */
	(void) pthread_join(forker_thr, NULL);

	/* Terminate the "counter" thread. */
	printf("%d in main: terminating the counter thread\n", getpid());
	(void) pthread_cancel(counter_thr);
	(void) pthread_mutex_destroy(&mutex);

	printf("%d in main: waiting for child %d to exit\n", getpid(), pid);
	(void) wait(NULL);
}
