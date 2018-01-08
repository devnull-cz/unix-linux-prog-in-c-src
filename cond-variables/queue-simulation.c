/*
 * Queue simulation. At every step, the producer randomly generates a message
 * or not, and at each step, the consumer randomly takes a message or not. One
 * condition variable and one mutex is used. Note that if we used semaphores,
 * we would need 2 of them to represent both "sleeping" states.
 *
 * There is a randomness factor used to have the queue get some items in it and
 * also to hit both boundary cases (empty and full queue) from time to time and
 * so that threads actualy get blocked on the condition variable.
 *
 * Compile with:
 *   Linux: gcc -pthread queue-simulation.c
 *   elsewhere: cc queue-simulation.c
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE 700	// needed for F_LOCK

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>

#define	DEFAULT_MAX	10

int queue /* number of items in the queue */, max /* queue capacity */;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *
producer(void *x)
{
	int rnd;

	while (1) {
		pthread_mutex_lock(&mutex);
		/* We can't insert a "message" when the queue is full. */
		while (queue == max) {
			(void) printf("Queue full, producer sleeping.\n");
			pthread_cond_wait(&cond, &mutex);
		}

		rnd = random() % 2;

		/*
		 * The queue was empty and we produced a "message". We must
		 * notify the consumer so it can start working on the queue
		 * again.
		 */
		if (queue == 0 && rnd == 1)
			pthread_cond_signal(&cond);

		queue = queue + rnd;
		pthread_mutex_unlock(&mutex);

		poll(NULL, 0, 95);
	}

	/* NOTREACHED */
}

void *
consumer(void *x)
{
	int rnd;

	while (1) {
		pthread_mutex_lock(&mutex);

		/* We can't get a "message" when there is none. */
		while (queue == 0) {
			pthread_cond_wait(&cond, &mutex);
		}

		rnd = random() % 2;

		/*
		 * If the queue was full and we pulled a "message" from it we
		 * will signal the producer so that it can start producing
		 * messages again.
		 */
		if (queue == max && rnd == 1) {
			(void) printf("Queue no longer full, signalling "
			    "producer.\n");
			pthread_cond_signal(&cond);
		}

		queue = queue - rnd;
		pthread_mutex_unlock(&mutex);

		poll(NULL, 0, 100);
	}

	/* NOTREACHED */
}

int
main(int argc, char **argv)
{
	int i;
	pthread_t t_prod, t_cons;

	if (argc == 2)
		max = atoi(argv[1]);
	else
		max = DEFAULT_MAX;

	/* Seed random device. */
	srandom(time(NULL));

	pthread_create(&t_prod, NULL, producer, NULL);
	pthread_create(&t_cons, NULL, consumer, NULL);

	/*
	 * Main thread.  Periodically print the "contents" of the queue.
	 */
	while (1) {
		pthread_mutex_lock(&mutex);
		printf(" |");
		for (i = 0; i < queue; ++i)
			putchar('.');
		for (i = queue; i < max; ++i)
			putchar(' ');
		printf("|\n");
		pthread_mutex_unlock(&mutex);
		poll(NULL, 0, 85); /* sleep 85 ms */
	}

	/* never reached */
	return (0);
}
