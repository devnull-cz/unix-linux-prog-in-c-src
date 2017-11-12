include ../Makefile.common

PROGS=getenv print-argv	print-env putenv return-256 return-negative-1

all: $(PROGS)

getenv: getenv.o
	$(CC) -o $@ $^

print-argv: print-argv.o
	$(CC) -o $@ $^

putenv: putenv.o
	$(CC) -o $@ $^

return-256: return-256.o
	$(CC) -o $@ $^

return-negative-1: return-negative-1.o
	$(CC) -o $@ $^

clean:
	rm -f *.o a.out $(PROG)
