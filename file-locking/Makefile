PROGS=fcntl-locking lock-unlock	lockf reader

all: $(PROGS)

fcntl-locking: fcntl-locking.o
	$(CC) -o $@ $^

lock-unlock: lock-unlock.o
	$(CC) -o $@ $^

lockf: lockf.o
	$(CC) -o $@ $^

reader: reader.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
