PROGS=suvreq

all: $(PROGS)

suvreq: suvreq.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
