/*
 * Demonstrate the behavior of static stack size with POSIX threads.
 *
 * Compile with:
 *   cc -pthread pthread-stack-overflow.c
 *
 * The first argument is the max recursion depth, the 2nd argument is the stack
 * size.
 *
 * Run with e.g.:
 *   ./a.out 1024
 *   ./a.out 1024 $(( 1024 * 1024 ))
 *
 * (c) vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <pthread.h>
#include <signal.h>

void
recurse(int n)
{
	/* Intentionally unused variable to make the stack bigger */
	char foo[1024];

	/* To silence the compiler warning */
	(void)foo;
	printf("%d ", n);
	fflush(stdout);

	if (n == 0) {
		printf("\n");
		return;
	}

	recurse(--n);
}

void *
thread(void *x)
{
	recurse((intptr_t)x);

	return (NULL);
}

int
main(int argc, char *argv[])
{
	pthread_t t;
	int max;
	pthread_attr_t attr;
	size_t stacksize;
	int e;

	if (argc < 2)
		errx(1, "Usage: %s <iterations> [stack_size]", argv[0]);

	max = atoi(argv[1]);

	(void) pthread_attr_init(&attr);
	(void) pthread_attr_getstacksize(&attr, &stacksize);
	printf("Default stack size = %zd bytes\n", stacksize);

	if (argc == 3) {
		stacksize = atoi(argv[2]);
		printf("Setting stack size to %zd bytes\n", stacksize);
		if ((e = pthread_attr_setstacksize(&attr, stacksize)) != 0)
			errx(1, "set stack size: %s", strerror(e));
		(void) pthread_attr_getstacksize(&attr, &stacksize);
		printf("Current stack size = %zd bytes\n", stacksize);
	}

	(void) pthread_create(&t, &attr, thread, (void *)(intptr_t)max);
	(void) pthread_join(t, NULL);

	return (0);
}
