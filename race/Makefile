PROGS=fcntl-fixed-race race sem-fixed-race posix-sem-fixed-race atomic-add

all: $(PROGS)

atomic-add: CSTD = c11
atomic-add: atomic-add.o
	$(CC) -o $@ $^ -lpthread

posix-sem-fixed-race: posix-sem-fixed-race.o
	$(CC) -o $@ $^ -lpthread

fcntl-fixed-race: fcntl-fixed-race.o
	$(CC) -o $@ $^

race: race.o
	$(CC) -o $@ $^

sem-fixed-race: sem-fixed-race.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS) atomic-add

include ../Makefile.common
