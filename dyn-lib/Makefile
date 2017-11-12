include ../Makefile.common

all: libmy.so ld-lazy dlopen

ld-lazy: ld-lazy.o
	$(CC) -o $@ $^ -lm

dlopen: dlopen.o
	$(CC) -o $@ $^ -ldl

libmy.o: CFLAGS+=-fPIC

libmy.so: libmy.o
	$(CC) -shared -o $@ $^

clean:
	rm -f *.o a.out libmy.so dlopen ld-lazy
