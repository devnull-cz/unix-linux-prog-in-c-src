PROGS=abi-main.o libabi-32.so libabi-64.so

all: $(PROGS) abi-main.o

libabi-32.o: CFLAGS+=-fPIC -m32
libabi-64.o: CFLAGS+=-fPIC -m64

libabi-32.so: libabi-32.o
	$(CC) -shared -m32 -o $@ $^

libabi-64.so: libabi-64.o
	$(CC) -shared -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
