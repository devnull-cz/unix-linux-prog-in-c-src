PROGS=busy-wait connect-only select write-select

all: $(PROGS)

busy-wait: busy-wait.o
	$(CC) -o $@ $^

connect-only: connect-only.o
	$(CC) -o $@ $^

select: select.o
	$(CC) -o $@ $^

write-select: write-select.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
