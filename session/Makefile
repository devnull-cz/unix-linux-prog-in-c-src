include ../Makefile.common

PROGS=getpgrp getppid getsid

all: $(PROGS)

getpgrp: getpgrp.o
	$(CC) -o $@ $^

getppid: getppid.o
	$(CC) -o $@ $^

getsid: getsid.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
