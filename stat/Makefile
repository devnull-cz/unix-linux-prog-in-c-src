PROGS=filetype stat stat012

all: $(PROGS)

filetype: filetype.o
	$(CC) -o $@ $^

stat: stat.o
	$(CC) -o $@ $^

stat012: stat012.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
