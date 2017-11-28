all: exit

exit: exit.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out exit

include ../Makefile.common
