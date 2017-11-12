include ../Makefile.common

PROGS=libmy.so ld-lazy dlopen

all: $(PROGS)

ld-lazy: ld-lazy.o
	$(CC) -o $@ $^ -lm

dlopen: dlopen.o
	$(CC) -o $@ $^ -ldl

libmy.o: CFLAGS+=-fPIC

libmy.so: libmy.o
	$(CC) -shared -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
