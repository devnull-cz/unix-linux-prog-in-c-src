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
 *   ./a.out 1024 `expr 1024 \* 1024`
 *
 * (c) vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700	// for sigfillset()

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <pthread.h>
#include <signal.h>

void
recurse(int n)
{
	char foo[1024];	/* intentional */

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

	if (argc < 2)
		errx(1, "usage: %s <int> [stack_size]", argv[0]);

	max = atoi(argv[1]);

	(void) pthread_attr_init(&attr);
	(void) pthread_attr_getstacksize(&attr, &stacksize);
	printf("default stack size = %zd bytes\n", stacksize);

	if (argc == 3) {
		stacksize = atoi(argv[2]);
		printf("setting stack size to %zd bytes\n", stacksize);
		(void) pthread_attr_setstacksize(&attr, stacksize);
		(void) pthread_attr_getstacksize(&attr, &stacksize);
		printf("current stack size = %zd bytes\n", stacksize);
	}

	(void) pthread_create(&t, &attr, thread, (void *)(intptr_t)max);

	(void) pthread_join(t, NULL);

	return (0);
}
