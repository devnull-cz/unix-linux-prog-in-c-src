/*
 * Demonstrate how pthread_atfork() works.  The before() handler is called from
 * the thread which called fork().  While the before() handler is running, all
 * the other threads are still running as well - that is important since they
 * need to eventually release the locks the before() handler is waiting for.
 *
 * Note that in order to see which threads call the handlers we are using array
 * of pthread_t values associated with the thread name.  The other alternative
 * would be to use thread specific variable.
 *
 * Note that the "counter" thread does not call any of the atfork handlers.
 *
 * vlada@devnull.cz
 */

#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	char name[16];
	pthread_t t;
} thread_info_t;

enum { FORKER = 0, COUNTER = 1, MAX = 2 } thread_idx;
pthread_mutex_t mutex;
thread_info_t threads[MAX];

/*
 * This is a scheme to deal with thread IDs in a transparent way - in general we
 * cannot rely on thread IDs to be of a known type (e.g. integers).
 */
char *
print_id(void)
{
	int i;

	for (i = 0; i < sizeof (threads) / sizeof (pthread_t); i++) {
		if (pthread_equal(pthread_self(), threads[i].t) != 0)
			return (threads[i].name);
	}

	return (NULL);
}

void *
forker(void *x)
{
	int i;
	pid_t pid;

	threads[FORKER].t = pthread_self();
	strncpy(threads[FORKER].name, "forker", sizeof (threads[FORKER].name));

	printf("%d/%s: forking\n", getpid(), print_id());
	pid = fork();
	if (pid == 0)
		printf("%d: I'm child\n", getpid());
	else
		printf("%d: I'm parent\n", getpid());

	for (i = 0; i < 3; ++i) {
		printf("%d: loop #%d\n", getpid(), i);
		sleep(1);
	}

	return (NULL);
}

void *
counter(void *arg)
{
	threads[COUNTER].t = pthread_self();
	strncpy(threads[COUNTER].name, "counter",
	    sizeof (threads[COUNTER].name));

	while (1) {
		printf(".");
		fflush(stdout);
		sleep(1);
	}

	return (NULL);
}

void
before(void)
{
	printf("%d/%s: the \"before\" handler in, grabbing the mutex...\n",
	    getpid(), print_id());
	if (pthread_mutex_lock(&mutex) != 0)
		printf("mutex lock failed\n");
	printf("%d/%s: the \"before\" handler out, got the mutex\n",
	    getpid(), print_id());
}

void
after(void)
{
	printf("%d/%s: the \"after\" handler in, releasing the mutex\n",
	    getpid(), print_id());
	if (pthread_mutex_unlock(&mutex) != 0)
		printf("mutex unlock failed\n");
	printf("%d/%s: the \"after\" handler out, mutex released\n",
	    getpid(), print_id());
}

int
main(void)
{
	pthread_t forker_thr, counter_thr;

	/*
	 * pthread_atfork() should be called after locks are initialized
	 * and before they are used.  Libraries which do this in .init section
	 * should use pthread_once().
	 */
	(void) pthread_mutex_init(&mutex, NULL);
	(void) pthread_atfork(before, after, after);

	if (pthread_mutex_lock(&mutex) != 0)
		printf("mutex lock failed\n");

	(void) pthread_create(&forker_thr, NULL, forker, NULL);
	(void) pthread_create(&counter_thr, NULL, counter, NULL);

	sleep(3);

	printf("main: unlocking mutex\n");
	if (pthread_mutex_unlock(&mutex) != 0)
		printf("mutex unlock failed\n");
	printf("%d in main: waiting for the threads\n", getpid());

	/* Wait for the "forker" thread to complete. */
	(void) pthread_join(forker_thr, NULL);

	/* Terminate the "counter" thread. */
	(void) pthread_cancel(counter_thr);
	(void) pthread_mutex_destroy(&mutex);

	return (0);
}
