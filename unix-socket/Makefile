PROGS=main

all: $(PROGS)

main: main.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
