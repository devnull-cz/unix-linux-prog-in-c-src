include ../Makefile.common

PROGS=readdir

all: $(PROGS)

readdir: readdir.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
