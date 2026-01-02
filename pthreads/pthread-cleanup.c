#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static void
cleanup(void *p)
{
	printf("cleanup ! cleanup ! everybody let's cleanup !\n");
}

void *
thread(void *x)
{
	for (unsigned int i = 0; i < 5; ++i) {
		int doit = 0;
		pthread_cleanup_push(cleanup, NULL);
		(void) printf("thread loop #%d\n", i);
		if (i > 2)
			doit = 1;
		(void) sleep(1);
		pthread_cleanup_pop(doit);
	}
	return (NULL);
}

int
main(int argc, char *argv[])
{
	pthread_t t;

	(void) pthread_create(&t, NULL, thread, NULL);
	pthread_join(t, NULL);

	return (0);
}
