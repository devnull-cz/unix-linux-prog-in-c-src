/*
 * Work with 2 global variables from 2 threads. At each loop each thread sets
 * both to its value passed in by pthread_create(). Before the assignment it
 * checks whether our variables are equal. If not, the thread prints its
 * identification character. There should be no races since both variables are
 * protected by a mutex. No races means no output.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int a, b;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *
thread(void *x)
{
	int data = (int) x;

	while (1) {
		pthread_mutex_lock(&mutex);
		/*
		 * If there is a race, print a character depending on
		 * which thread is experiencing the inconsistency.
		 */
		if (a != b) {
			if (data == 0)
				write(1, "|", 1);
			else
				write(1, "_", 1);
		}

		a = data;
		b = data;
		pthread_mutex_unlock(&mutex);
	}

	return (NULL);
}

int
main(void)
{
	pthread_t t1, t2;

	pthread_create(&t1, NULL, thread, (void *) 0);
	pthread_create(&t2, NULL, thread, (void *) 1);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return (0);
}
