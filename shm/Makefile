PROGS=shm

all: $(PROGS)

shm: shm.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
