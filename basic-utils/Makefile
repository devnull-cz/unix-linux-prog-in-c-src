include ../Makefile.common

all: standards suvreq lineinfo main

standards: standards.o
	$(CC) -o $@ $^

suvreq: suvreq.o
	$(CC) -o $@ $^

lineinfo: main__LINE__.o
	$(CC) -o $@ $^

main: main.o common.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out assert suvreq lineinfo main
