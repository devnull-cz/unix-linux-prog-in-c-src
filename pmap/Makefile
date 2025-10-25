PROGS=proc-addr-space stack-overflow

all: $(PROGS)

proc-addr-space: proc-addr-space.o
	$(CC) -o $@ $^

stack-overflow: stack-overflow.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
