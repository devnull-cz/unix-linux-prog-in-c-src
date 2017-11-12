include ../Makefile.common

PROGS=getopt

all: $(PROGS)

getopt: getopt.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROG)
