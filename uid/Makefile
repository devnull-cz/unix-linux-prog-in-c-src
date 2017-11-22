include ../Makefile.common

PROGS=getuids setreuid setreuid2 setuid

all: $(PROGS)

getuids: getuids.o
	$(CC) -o $@ $^

setreuid: setreuid.o
	$(CC) -o $@ $^

setreuid2: setreuid2.o
	$(CC) -o $@ $^

setuid: setuid.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
