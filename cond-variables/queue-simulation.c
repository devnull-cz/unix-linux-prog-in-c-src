/*
 * A queue simulation.  At every interval, the producer randomly generates a
 * message or not, and at each step, the consumer randomly takes a message or
 * not.  One condition variable and one mutex is used.  Note that if we used
 * semaphores, we would need 2 of them to represent both "sleeping" states.
 *
 * A randomness factor is used here to have the queue get some items in it and
 * also to hit both boundary cases (empty and full queue) from time to time.
 * When hitting a boundary, one of the thread actualy gets blocked on the
 * condition variable.
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

/* number of items in the queue */
int in_queue;
/* queue capacity */
int capacity = DEFAULT_MAX;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *
producer(void *x)
{
	int msg_created;

	while (1) {
		pthread_mutex_lock(&mutex);
		/* We can't insert a "message" when the queue is full. */
		while (in_queue == capacity) {
			(void) printf("Queue already full, producer "
			    "sleeping.\n");
			pthread_cond_wait(&cond, &mutex);
		}

		msg_created = random() % 2;

		in_queue = in_queue + msg_created;
		if (in_queue == capacity)
			(void) printf("Note: queue just filled up.\n");

		/*
		 * If the queue was empty and we produced a "message",  we must
		 * notify the consumer so it can start working on the queue
		 * again.  Note that the consumer actually may or may not sleep
		 * if the queue was empty - it may yet not sleep if it didn't
		 * get to check the queue again to find it empty.
		 *
		 * We could also signal each time we insert a message but that
		 * is really not necessary and would be actually wasteful.
		 */
		if (in_queue == 1 && msg_created == 1) {
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&cond);
		} else {
			pthread_mutex_unlock(&mutex);
		}

		poll(NULL, 0, 95);
	}

	/* NOTREACHED */
}

void *
consumer(void *x)
{
	int msg_removed;

	while (1) {
		pthread_mutex_lock(&mutex);

		/* We cannot get a "message" when there is none so go to sleep. */
		while (in_queue == 0) {
			pthread_cond_wait(&cond, &mutex);
		}

		msg_removed = random() % 2;

		/*
		 * If the queue was full and we pulled a "message" from it we
		 * will signal the producer so that it can start producing
		 * messages again.
		 */
		in_queue = in_queue - msg_removed;
		if (in_queue == 0)
			(void) printf("Note: queue just emptied.\n");
		if ((in_queue + msg_removed) == capacity && msg_removed == 1) {
			(void) printf("Queue no longer full, signalling "
			    "producer.\n");
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&cond);
		} else {
			pthread_mutex_unlock(&mutex);
		}

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
		capacity = atoi(argv[1]);

	/* Seed the random device. */
	srandom(time(NULL));

	pthread_create(&t_prod, NULL, producer, NULL);
	pthread_create(&t_cons, NULL, consumer, NULL);

	/*
	 * Main thread.  Periodically print the "contents" of the queue.  A dot
	 * means an item, a space means an empty slot.
	 */
	while (1) {
		pthread_mutex_lock(&mutex);
		printf(" |");
		for (i = 0; i < in_queue; ++i)
			putchar('.');
		for (i = in_queue; i < capacity; ++i)
			putchar(' ');
		printf("|\n");
		pthread_mutex_unlock(&mutex);
		poll(NULL, 0, 85); /* sleep 85 ms */
	}

	/* Never reached. */
	return (0);
}
