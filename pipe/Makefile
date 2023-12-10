ALL=broken-pipe \
    deadlock-in-read \
    pipe-and-fork \
    countdown

all: $(ALL)

broken-pipe: broken-pipe.o
	$(CC) -o $@ $<

deadlock-in-read: deadlock-in-read.o
	$(CC) -o $@ $<

pipe-and-fork: pipe-and-fork.o
	$(CC) -o $@ $<

countdown: countdown.o
	$(CC) -o $@ $<

clean:
	rm -f *.o a.out $(ALL)

include ../Makefile.common
