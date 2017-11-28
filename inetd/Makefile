PROGS=accept-rw

all: $(PROGS)

accept-rw: accept-rw.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROG)

include ../Makefile.common
