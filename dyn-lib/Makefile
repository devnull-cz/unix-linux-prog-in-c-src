PROGS=libmy dlopen

ifeq ($(UNAME_S),Linux)
  PROGS+=ld-lazy
endif

all: $(PROGS)

ld-lazy: ld-lazy.o
	$(CC) -o $@ $^ -lm

dlopen: dlopen.o
	$(CC) -o $@ $^ -ldl

libmy.o: CFLAGS+=-fPIC

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common

.PHONY: libmy

libmy: libmy.$(LIB_EXT)

libmy.$(LIB_EXT): libmy.o
	$(CC) $(CFLAGS_DYNLIB) $(CFLAGS_DYNLIB_EXTRA) -o $@ $^

ifeq ($(UNAME_S),Darwin)
  CFLAGS_DYNLIB_EXTRA=-Wl,-undefined,dynamic_lookup
endif
