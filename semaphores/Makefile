PROGS=posix-unnamed-sem \
      talking-heads

all: $(PROGS)

posix-unnamed-sem: posix-unnamed-sem.o
	$(CC) -o $@ $^ -lpthread

talking-heads: talking-heads.o
	$(CC) -o $@ $^ -lpthread

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
