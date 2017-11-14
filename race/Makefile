include ../Makefile.common

PROGS=atomic-add fcntl-fixed-race loop.o race sem-fixed-race

all: $(PROGS)

atomic-add: atomic-add.o
	$(CC) -o $@ $^ -lpthread

fcntl-fixed-race: fcntl-fixed-race.o
	$(CC) -o $@ $^

race: race.o
	$(CC) -o $@ $^

sem-fixed-race: sem-fixed-race.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
