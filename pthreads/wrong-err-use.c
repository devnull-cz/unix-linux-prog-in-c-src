/*
 * Wrong use of the err(3) function.  As the Pthread API does not set errno,
 * using err() is incorrect.  However, on different system, the message it
 * prints may not be very helpful (try on Linux).
 */
#include <err.h>
#include <pthread.h>

void *
thread(void *x)
{
	return (NULL);
}

int
main(void)
{
	pthread_t t;

	/* We are lazy and assume this succeeds. */
	(void) pthread_create(&t, NULL, thread, NULL);
	/* We are lazy and assume this succeeds. */
	(void) pthread_join(t, NULL);
	/* This should fail as the thread has been already joined. */
	if (pthread_join(t, NULL) != 0)
		err(1, "pthread_join");
	return (0);
}
