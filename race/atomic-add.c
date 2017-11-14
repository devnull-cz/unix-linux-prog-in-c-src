/*
 * Use time(1) to measure all three different modes of execution - no option,
 * -a option and -m option. Use multiprocessor machine if possible.
 *
 * The program creates two threads and let them each sum up 1+2+3+..+<n>, using
 * a shared 32 bit integer variable.  So, if increment is an atomic operation,
 * we should get twice that sum.  If the increment operation is not atomic, we
 * get something else.  We do not worry about overflows.
 *
 * Tested on UltraSparc T1000 with num_of_cycles = 99999999
 *
 * On this (or similar architecture where operation 'ADD' is not atomic)
 * machine, you should see RACE for execution without any option and correct
 * behaviour for both -a and -m. However, using -m is much slower (almost 10x
 * for my tests) than -a solution. That is due to the overhead of mutexes in
 * contrast to atomic_add() which can be used if the only access to a shared
 * data is to increase or decrease its value.
 *
 * I caught a race also on 1-CPU amd64 machine.
 *
 * Compile with:
 *   cc atomic-add.c
 *
 * This example currently works on Solaris and systems with C11 compiler.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700	// for getopt()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#ifndef __STDC_NO_ATOMICS__
#include <stdatomic.h>
#define	ATOMIC_ADD(ptr, incr)	atomic_fetch_add(ptr, incr)
atomic_int x;
#else
#if defined(__sun) && defined(__SVR4)
#include <atomic.h>
#define	ATOMIC_ADD(ptr, incr)	atomic_add_32(ptr, incr)
uint32_t x;
#else
#error "This will not work"
#endif
#endif

int atomic, mutex;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void
usage(void)
{
	fprintf(stderr, "atomic/mutex/race tester");
	fprintf(stderr, "\n");
	fprintf(stderr, "a.out [-a|-m] <num_of_cycles>\n");
	fprintf(stderr, "	-a	use atomic_add(3c)/C11 stdatomic\n");
	fprintf(stderr, "	-m	use pthread_mutex(3c)\n");
	fprintf(stderr, "	-h	this help\n");
}

void *
count(void *arg)
{
	int i, n = *((int *)arg);

	if (atomic) {
		for (i = 1; i < n; ++i)
			ATOMIC_ADD(&x, i);
	} else {
		if (mutex) {
			for (i = 1; i < n; ++i) {
				pthread_mutex_lock(&m);
				x = x + i;
				pthread_mutex_unlock(&m);
			}
		} else {
			for (i = 1; i < n; ++i)
				x = x + i;
		}
	}

	return (NULL);
}

int
main(int argc, char **argv)
{
	unsigned int i, n1, n2, x2;
	pthread_t t1, t2;
	char c;

	while ((c = getopt(argc, argv, "ahm")) != EOF) {
		switch (c) {
		case 'a':
			atomic = 1;
			break;
		case 'm':
			mutex = 1;
			break;
		case 'h':
		case '?':
		default:
			usage();
			exit(1);
			break;
		}
	}

	if ((atomic + mutex) == 2 || argc != (optind + 1)) {
		usage();
		exit(1);
	}

	n2 = n1 = atoi(argv[optind]);

	/*
	 * And don't worry about overflows, we just look for races, not the
	 * correct sum.  With unsigned int (32 bits), we get the overflow for n
	 * ~ 92k.
	 */
	pthread_create(&t1, NULL, count, (void *)&n1);
	pthread_create(&t2, NULL, count, (void *)&n2);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	/*
	 * Count the sum for one thread.  Do exactly what we would do in the
	 * threads.
	 */
	(void) printf("n is %d.\n", n1);
	for (i = 1, x2 = 0; i < n1; ++i)
		x2 = x2 + i;

	/* If we do not get twice the sum, we hit a race. */
	if (x != 2 * x2)
		printf("RACE DETECTED\n");
	else
		printf("everything OK, no race detected\n");

	printf("result is %u\n", x);
	printf("expected %u\n", 2 * x2);

	return (0);
}
