include ../Makefile.common

PROGS=sem

all: $(PROGS)

sem: sem.o
	$(CC) -o $@ $^ -lpthread

clean:
	rm -f *.o a.out $(PROGS)
