include ../Makefile.common

PROGS=udp-client udp-server

all: $(PROGS)

udp-client: udp-client.o
	$(CC) -o $@ $^

udp-server: udp-server.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)
