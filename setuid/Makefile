PROGS=screate-file

all: $(PROGS)

screate-file: screate-file.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
