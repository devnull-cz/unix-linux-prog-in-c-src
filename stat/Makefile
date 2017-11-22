include ../Makefile.common

PROGS=filetype stat

all: $(PROGS)

filetype: filetype.o
	$(CC) -o $@ $^

stat: stat.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
