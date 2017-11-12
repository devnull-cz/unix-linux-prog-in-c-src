include ../Makefile.common

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
