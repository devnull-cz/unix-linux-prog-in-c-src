include ../Makefile.common

ALL=broken-pipe \
    deadlock-in-read \
    pipe-and-fork \
    pipe-sig

all: $(ALL)

broken-pipe: broken-pipe.o
	$(CC) -o $@ $<

deadlock-in-read: deadlock-in-read.o
	$(CC) -o $@ $<

pipe-and-fork: pipe-and-fork.o
	$(CC) -o $@ $<

pipe-sig: pipe-sig.o
	$(CC) -o $@ $<

clean:
	rm -f *.o a.out $(ALL)
