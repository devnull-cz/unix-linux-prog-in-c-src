include ../Makefile.common

PROGS=not-my-lock race-fixed race

all: $(PROGS)

not-my-lock: not-my-lock.o
	$(CC) -o $@ $^ -lpthread

race-fixed: race-fixed.o
	$(CC) -o $@ $^ -lpthread

race: race.o
	$(CC) -o $@ $^ -lpthread

clean:
	rm -f *.o a.out $(PROGS)
