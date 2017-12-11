PROGS=connect \
	linger \
	reuseaddr \
	tcp-sink-server \
	trivial-tcp-echo-server \
	up-to-listen-only

all: $(PROGS)

connect: connect.o
	$(CC) -o $@ $^

linger: linger.o
	$(CC) -o $@ $^

reuseaddr: reuseaddr.o
	$(CC) -o $@ $^

tcp-sink-server: tcp-sink-server.o
	$(CC) -o $@ $^

trivial-tcp-echo-server: trivial-tcp-echo-server.o
	$(CC) -o $@ $^

up-to-listen-only: up-to-listen-only.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common
