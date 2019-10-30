PROGS=unlink

all: $(PROGS)

unlink: unlink.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
