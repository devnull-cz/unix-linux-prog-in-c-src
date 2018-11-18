PROGS=sem \
      talking-heads

all: $(PROGS)

sem: sem.o
	$(CC) -o $@ $^ -lpthread

talking-heads: talking-heads.o
	$(CC) -o $@ $^ -lpthread

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
