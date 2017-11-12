include ../Makefile.common

all: coredump

assert: coredump.o
	$(CC) -o $@ $<

clean:
	rm -f *.o a.out coredump
