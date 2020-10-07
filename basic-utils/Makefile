# Search for "gmake automatic variables" to learn more.
#
# However, this Makefile is part of the make system that builds the whole
# unix-linux-programming-in-c-src repository.

all: standards suvreq lineinfo main libpreload.so

standards: standards.o
	$(CC) -o $@ $^

suvreq: suvreq.o
	$(CC) -o $@ $^

lineinfo: main__LINE__.o
	$(CC) -o $@ $^

main: main.o common.o
	$(CC) -o $@ $^

preload.o: preload.c
	$(CC) $(CFLAGS) -fPIC -c $^

libpreload.so: preload.o
	$(CC) -shared -o $@ $^

clean:
	rm -f *.o a.out assert suvreq lineinfo main libpreload.so

include ../Makefile.common
