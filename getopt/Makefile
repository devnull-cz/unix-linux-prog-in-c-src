PROGS=getopt getopt_long

all: $(PROGS)

getopt: getopt.o
	$(CC) -o $@ $^

getopt_long: getopt_long.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
