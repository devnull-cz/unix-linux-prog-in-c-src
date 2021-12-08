all: $(PROGS)

include ../Makefile.common

ifeq ($(UNAME_S),Darwin)
PROGS=
else
PROGS=abi-main-32 abi-main-64
endif

ifeq ($(UNAME_S),Darwin)
	MAIN_LDFLAGS=-Xlinker -rpath -Xlinker .
else
	MAIN_LDFLAGS=-Xlinker -R=.
endif

CFLAGS+=-fPIC -m32

abi-main-32: abi-main.o libabi-32.$(LIB_EXT)
	$(CC) $(CFLAGS) -o $@ abi-main.o -L. $(MAIN_LDFLAGS) -labi-32

abi-main-64: abi-main.o libabi-64.$(LIB_EXT)
	$(CC) $(CFLAGS) -o $@ abi-main.o -L. $(MAIN_LDFLAGS) -labi-64

libabi-32.$(LIB_EXT): libabi-32.o
	$(CC) -m32 $(CFLAGS_DYNLIB) -o $@ $^

libabi-64.$(LIB_EXT): libabi-64.o
	$(CC) -m32 $(CFLAGS_DYNLIB) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

