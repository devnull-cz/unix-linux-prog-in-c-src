include ../Makefile.common

PROGS=aligned close lseek override reverse sigbus map-nocore

all: $(PROGS)

aligned: aligned.o
	$(CC) -o $@ $^

close: close.o
	$(CC) -o $@ $^

lseek: CFLAGS += -g
lseek: lseek.o
	$(CC) -o $@ $^

override: override.o
	$(CC) -o $@ $^

reverse: reverse.o
	$(CC) -o $@ $^

sigbus: sigbus.o
	$(CC) -o $@ $^

map-nocore: map-nocore.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
