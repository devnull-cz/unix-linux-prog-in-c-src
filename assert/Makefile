include ../Makefile.common

all: assert

assert: assert.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out assert
