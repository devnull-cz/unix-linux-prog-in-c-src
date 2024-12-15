/*
 * From two threads, work with members of a global variable.  At every loop,
 * each thread sets both members to the value passed in by pthread_create().
 * Before the assignment it checks whether the structure members are equal.
 * If not, the thread prints its identification character.
 * You should see a lot of races even on a single CPU system.
 *
 * Any character printed means a race.
 *
 * (c) jp@devnull.cz
 */

#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct {
	int a;
	int b;
} foo;

void *
thread(void *x)
{
	intptr_t data = (intptr_t)x;

	while (1) {
		if (foo.a != foo.b) {
			if (data == 0)
				write(1, "|", 1);
			else
				write(1, "_", 1);
		}

		foo.a = data;
		foo.b = data;
	}

	return (NULL);
}

int
main(void)
{
	pthread_t t1, t2;

	pthread_create(&t1, NULL, thread, (void *)0);
	pthread_create(&t2, NULL, thread, (void *)1);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return (0);
}
