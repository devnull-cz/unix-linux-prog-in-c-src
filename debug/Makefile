all: coredump dbx crash-static-string

assert: coredump.o
	$(CC) -o $@ $<

dbx: dbx.o
	$(CC) -o $@ $<

crash-static-string: crash-static-string.o
	$(CC) -o $@ $<

clean:
	rm -f *.o a.out coredump dbx crash-static-string

include ../Makefile.common
