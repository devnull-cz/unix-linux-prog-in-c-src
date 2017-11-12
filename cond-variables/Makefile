include ../Makefile.common

all: queue-simulation

queue-simulation: queue-simulation.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out queue-simulation
