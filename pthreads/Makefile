PROGS=	atfork \
	correct-use-of-args \
	fork \
	fork-not-in-main \
	implement-barrier \
	implement-barrier-fixed \
	int-as-arg \
	pthread-cancel \
	pthread-create \
	pthread-detach-join \
	pthread-join \
	pthread-stack-overflow \
	pthread-rwlock \
	set-detachstate \
	setjmp \
	sigwait \
	sigwait-with-sync-signals \
	wrong-err-use \
	wrong-use-of-arg

all: $(PROGS)

LDFLAGS += -lpthread

atfork: atfork.o
	$(CC) -o $@ $^ $(LDFLAGS)

correct-use-of-args: correct-use-of-args.o
	$(CC) -o $@ $^ $(LDFLAGS)

# Works on Solaris only, you must build it manually.
forkall: forkall.o
	$(CC) -o $@ $^ $(LDFLAGS)

fork: fork.o
	$(CC) -o $@ $^ $(LDFLAGS)

fork-not-in-main: fork-not-in-main.o
	$(CC) -o $@ $^ $(LDFLAGS)

implement-barrier: implement-barrier.o
	$(CC) -o $@ $^ $(LDFLAGS)

implement-barrier-fixed: implement-barrier-fixed.o
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

pthread-rwlock: pthread-rwlock.o
	$(CC) -o $@ $^ $(LDFLAGS)

set-detachstate: set-detachstate.o
	$(CC) -o $@ $^ $(LDFLAGS)

setjmp: setjmp.o
	$(CC) -o $@ $^ $(LDFLAGS)

sigwait: sigwait.o
	$(CC) -o $@ $^ $(LDFLAGS)

sigwait-with-sync-signals: sigwait-with-sync-signals.o
	$(CC) -o $@ $^ $(LDFLAGS)

wrong-err-use: wrong-err-use.o
	$(CC) -o $@ $^ $(LDFLAGS)

wrong-use-of-arg: wrong-use-of-arg.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o a.out $(PROGS)

include ../Makefile.common

# macOS does not implement barriers API.
ifneq ($(UNAME_S),Darwin)
  PROGS+=pthread-barrier
endif
