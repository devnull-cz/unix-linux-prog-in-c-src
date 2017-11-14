include ../Makefile.common

PROGS=	atfork \
	correct-use-of-args \
	fork \
	fork-not-in-main \
	int-as-arg \
	pthread-cancel \
	pthread-create \
	pthread-detach-join \
	pthread-join \
	pthread-stack-overflow \
	set-detachstate \
	setjmp \
	sigwait \
	sigwait-with-sync-signals \
	wrong-use-of-arg

all: $(PROGS)

LDFLAGS += -lpthread

clean:
	rm -f *.o a.out $(PROGS)

atfork: atfork.o
	$(CC) -o $@ $^ $(LDFLAGS)

correct-use-of-args: correct-use-of-args.o
	$(CC) -o $@ $^ $(LDFLAGS)

# works on Solaris only
forkall: forkall.o
	$(CC) -o $@ $^ $(LDFLAGS)

fork: fork.o
	$(CC) -o $@ $^ $(LDFLAGS)

fork-not-in-main: fork-not-in-main.o
	$(CC) -o $@ $^ $(LDFLAGS)

int-as-arg: int-as-arg.o
	$(CC) -o $@ $^ $(LDFLAGS)

pthread-barrier: pthread-barrier.o
	$(CC) -o $@ $^ $(LDFLAGS)

pthread-cancel: pthread-cancel.o
	$(CC) -o $@ $^ $(LDFLAGS)

pthread-create: pthread-create.o
	$(CC) -o $@ $^ $(LDFLAGS)

pthread-detach-join: pthread-detach-join.o
	$(CC) -o $@ $^ $(LDFLAGS)

pthread-join: pthread-join.o
	$(CC) -o $@ $^ $(LDFLAGS)

pthread-stack-overflow: pthread-stack-overflow.o
	$(CC) -o $@ $^ $(LDFLAGS)

set-detachstate: set-detachstate.o
	$(CC) -o $@ $^ $(LDFLAGS)

setjmp: setjmp.o
	$(CC) -o $@ $^ $(LDFLAGS)

sigwait: sigwait.o
	$(CC) -o $@ $^ $(LDFLAGS)

sigwait-with-sync-signals: sigwait-with-sync-signals.o
	$(CC) -o $@ $^ $(LDFLAGS)

wrong-use-of-arg: wrong-use-of-arg.o
	$(CC) -o $@ $^ $(LDFLAGS)
