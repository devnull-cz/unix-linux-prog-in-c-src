include ../Makefile.common

PROGS=sem \
      posix-sem-fixed-race

all: $(PROGS)

sem: sem.o
	$(CC) -o $@ $^ -lpthread

posix-sem-fixed-race: posix-sem-fixed-race.o
	$(CC) -o $@ $^ -lpthread

clean:
	rm -f *.o a.out $(PROGS)
