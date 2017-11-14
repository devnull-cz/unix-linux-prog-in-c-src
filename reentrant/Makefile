include ../Makefile.common

PROGS=gethostbyname inet_ntoa

all: $(PROGS)

gethostbyname: gethostbyname.o
	$(CC) -o $@ $^

inet_ntoa: inet_ntoa.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
