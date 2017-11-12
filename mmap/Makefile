include ../Makefile.common

PROGS=aligned close lseek override reverse sigbus

all: $(PROGS)

aligned: aligned.o
	$(CC) -o $@ $^

close: close.o
	$(CC) -o $@ $^

lseek: lseek.o
	$(CC) -o $@ $^

override: override.o
	$(CC) -o $@ $^

reverse: reverse.o
	$(CC) -o $@ $^

sigbus: sigbus.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
