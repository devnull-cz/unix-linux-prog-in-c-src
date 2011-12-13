/*
 * Run two processes that share 2 bytes of memory. Both of them loop until
 * a signal is received, while changing both bytes to their respective value (1
 * or 2). Before setting those bytes a check is performed whether bytes are
 * equal. If not we have a race. After you kill it with Ctrl-C you will see some
 * statistics. Run with any argument to see races as they happen.
 *
 * Note that with a job control, Ctrl-C makes the shell to send a SIGINT signal
 * to the whole group so we don't have to worry about killing just a father.
 *
 * (c) jp@devnull.cz
 */

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <err.h>

unsigned long i;	/* number of loops per process, u_long should be enough for basic demo */
unsigned int j;		/* j is number of races detected */

void
print_stat(int sig)
{
	printf("\nstats: inconsistency %u of %u\n", j, i);
	_exit(0);
}

int
main(int argc, char **argv)
{
	char c = 0;
	int fd, dbg = 0;
	char *addr = NULL;
	struct sigaction act;

	if (argc == 1)
		printf("run with any argument to see some debug info\n");
	else
		dbg = 1;

	bzero(&act, sizeof(act));
	act.sa_handler = print_stat;
	sigaction(SIGINT, &act, NULL);

	if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
		err(1, "open");

	/* extend the file to 2 bytes */
	write(fd, &c, 1);
	write(fd, &c, 1);

	addr = mmap(0, 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((void *)addr == MAP_FAILED)
		err(1, "mmap");

	switch (fork()) {
	case -1:
		err(1, "fork");
	case 0: 
		while (1) {
			if (addr[0] != addr[1]) {
				if (dbg)
					fprintf(stderr, "[child (%d/%d)] ",
					    addr[0], addr[1]);
				++j;
			}
			addr[0] = addr[1] = 2;
			++i;
		}
		break;
	default:
		while (1) {
			if (addr[0] != addr[1]) {
				if (dbg)
					fprintf(stderr, "[PARENT (%d/%d)] ",
					    addr[0], addr[1]);
				++j;
			}
			addr[0] = addr[1] = 1;
			++i;
		}
		break;
	}

	munmap(addr, 2);
	close(fd);

	return (0);
}
