all: self-dead-lock

self-dead-lock: self-dead-lock.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out self-dead-lock

include ../Makefile.common
