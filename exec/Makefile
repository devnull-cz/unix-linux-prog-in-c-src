include ../Makefile.common

PROGS=exec-date spawn

all: $(PROGS)

exec-date: exec-date.o
	$(CC) -o $@ $^

spawn: spawn.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
