PROGS=big-file cat isstdout lseek redirect redirect2 simple-cat

all: $(PROGS)

big-file: big-file.o
	$(CC) -o $@ $^

cat: cat.o
	$(CC) -o $@ $^

isstdout: isstdout.o
	$(CC) -o $@ $^

lseek: lseek.o
	$(CC) -o $@ $^

redirect: redirect.o
	$(CC) -o $@ $^

redirect2: redirect2.o
	$(CC) -o $@ $^

simple-cat: simple-cat.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
