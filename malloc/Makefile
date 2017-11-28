PROGS=brk

all: $(PROGS)

brk: brk.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
