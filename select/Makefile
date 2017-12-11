PROGS=busy-wait connect-only select write-select non-blocking-connect

all: $(PROGS)

busy-wait: busy-wait.o
	$(CC) -o $@ $^

connect-only: connect-only.o
	$(CC) -o $@ $^

select: select.o
	$(CC) -o $@ $^

write-select: write-select.o
	$(CC) -o $@ $^

non-blocking-connect: non-blocking-connect.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
