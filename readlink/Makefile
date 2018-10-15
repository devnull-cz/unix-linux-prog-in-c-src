PROGS=readlink

all: $(PROGS)

readlink: readlink.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
