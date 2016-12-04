/*
 * Demonstrate how pthread_atfork() works. The before() handler called from
 * the thread which called fork(). While the before() handler is running,
 * all the other threads still keep running - this is important since they
 * need to eventually release the locks the handler is waiting for.
 *
 * Note that in order to see which threads call the handlers we are using array
 * of pthread_t values associated with thread name.  The other alternative would
 * be to use thread specific variable.
 *
 * Note that the "counter" thread does not call any of the atfork handlers.
 *
 * vlada@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <poll.h>

typedef struct {
	char name[16];
	pthread_t t;
} thread_info_t;

enum { FORKER = 0, COUNTER = 1, MAX = 2 } thread_idx;
pthread_mutex_t mutex;
thread_info_t threads[MAX];

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
		poll(NULL, 0, 10);	/* sleep 10 ms */
	}

	return (NULL);
}

void
before(void)
{
	printf("%d/%s: before handler in\n", getpid(), print_id());
	if (pthread_mutex_lock(&mutex) != 0)
		printf("mutex lock failed\n");
	printf("%d/%s: before handler out\n", getpid(), print_id());
}

void
after(void)
{
	printf("%d/%s: after handler in\n", getpid(), print_id());
	if (pthread_mutex_unlock(&mutex) != 0)
		printf("mutex unlock failed\n");
	printf("%d/%s: after handler out\n", getpid(), print_id());
}

int
main(void)
{
	pthread_t t1, t2;

	pthread_mutex_init(&mutex, NULL);
	if (pthread_mutex_lock(&mutex) != 0)
		printf("mutex lock failed\n");

	pthread_atfork(before, after, after);
	pthread_create(&t1, NULL, forker, NULL);
	pthread_create(&t2, NULL, counter, NULL);

	sleep(3);

	printf("main: unlocking mutex\n");
	if (pthread_mutex_unlock(&mutex) != 0)
		printf("mutex unlock failed\n");
	printf("%d in main: waiting for the threads\n", getpid());

	/* Wait for the "forker" thread to complete. */
	pthread_join(t1, NULL);

	/* Terminate the "counter" thread. */
	pthread_cancel(t2);

	pthread_mutex_destroy(&mutex);

	return (0);
}
