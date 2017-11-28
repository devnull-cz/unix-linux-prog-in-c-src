PROGS=fork vfork

all: $(PROGS)

fork: fork.o
	$(CC) -o $@ $^

vfork: vfork.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROG)

include ../Makefile.common
