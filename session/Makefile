PROGS=getppid

all: $(PROGS)

getppid: getppid.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
