PROGS=wait

all: $(PROGS)

wait: wait.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
