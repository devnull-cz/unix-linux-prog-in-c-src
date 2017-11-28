all: queue-simulation

queue-simulation: queue-simulation.o
	$(CC) -o $@ $^ -lpthread

clean:
	rm -f *.o a.out queue-simulation

include ../Makefile.common
