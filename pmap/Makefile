PROGS=proc-addr-space

all: $(PROGS)

proc-addr-space: proc-addr-space.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
