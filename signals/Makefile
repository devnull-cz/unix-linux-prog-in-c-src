PROGS=alarm \
	catch-all \
	catch-SIGKILL \
	catch-SIGSEGV \
	check-existence \
	ctrl_c \
	div-by-zero \
	event-loop \
	interrupted-read \
	kill-and-catch \
	killing-myself \
	ping-pong \
	siginfo \
	signal-vs-sigaction \
	sigsegv \
	sigwait \
	sigpending \
	sigchld

all: $(PROGS)

alarm: alarm.o
	$(CC) -o $@ $^

catch-all: catch-all.o
	$(CC) -o $@ $^

catch-SIGKILL: catch-SIGKILL.o
	$(CC) -o $@ $^ -lpthread

catch-SIGSEGV: catch-SIGSEGV.o
	$(CC) -o $@ $^

check-existence: check-existence.o
	$(CC) -o $@ $^

ctrl_c: ctrl_c.o
	$(CC) -o $@ $^

div-by-zero: div-by-zero.o
	$(CC) -o $@ $^

event-loop: event-loop.o
	$(CC) -o $@ $^

interrupted-read: interrupted-read.o
	$(CC) -o $@ $^

kill-and-catch: kill-and-catch.o
	$(CC) -o $@ $^

killing-myself: killing-myself.o
	$(CC) -o $@ $^

ping-pong: ping-pong.o
	$(CC) -o $@ $^

siginfo: siginfo.o
	$(CC) -o $@ $^

signal-vs-sigaction: signal-vs-sigaction.o
	$(CC) -o $@ $^

sigqueue: sigqueue.o
	$(CC) -o $@ $^

sigsegv: sigsegv.o
	$(CC) -o $@ $^

sigwait: sigwait.o
	$(CC) -o $@ $^

sigpending: sigpending.o
	$(CC) -o $@ $^

sigchld: sigchld.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common

ifneq ($(UNAME_S),Darwin)
  PROGS+=sigqueue
endif
