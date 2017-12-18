/*
 * Wrong use of the err(3) function.  As the Pthread API does not set errno,
 * using err() is incorrect.  However, on different system, the message it
 * prints may not be very helpful (try on Linux).
 */
#include <err.h>
#include <pthread.h>

int
main(void)
{
	/* This should fail but it is not guaranteed. */
	if (pthread_join((pthread_t)NULL, NULL) != 0)
		err(1, "pthread_join");
	return (0);
}
