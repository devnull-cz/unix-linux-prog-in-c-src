PROGS=nanosleep poll-sleep

all: $(PROGS)

nanosleep: nanosleep.o
	$(CC) -o $@ $^

poll-sleep: poll-sleep.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
