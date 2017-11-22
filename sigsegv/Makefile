include ../Makefile.common

PROGS=main main2 main3

all: $(PROGS)

main: main.o
	$(CC) -o $@ $^

main2: main2.o
	$(CC) -o $@ $^

main3: main3.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
