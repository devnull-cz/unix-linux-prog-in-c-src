all: err

err: err.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out err

include ../Makefile.common
