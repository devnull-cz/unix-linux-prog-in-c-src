PROGS=exec-date execl-buggy spawn

all: $(PROGS)

exec-date: exec-date.o
	$(CC) -o $@ $^

execl-buggy: execl-buggy.o
	$(CC) -o $@ $^

spawn: spawn.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
