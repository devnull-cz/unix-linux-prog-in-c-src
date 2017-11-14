include ../Makefile.common

# TODO: enable atomic-add once Travis has gcc 4.9 (issue #9)
PROGS=fcntl-fixed-race loop.o race sem-fixed-race

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
