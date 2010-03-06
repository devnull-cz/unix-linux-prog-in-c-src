/*
 * Two processes share a memory and they synchronize with SIGUSR1. When the
 * father reads a character from the terminal, it writes as the 1st character of
 * the shared memory and sends a signal to its child.
 *
 * The child is waiting in sigwait(). When the signals comes it reads the memory
 * and prints the character to stdout. Note that we use sigwait() which doesn't
 * use handlers.
 *
 * On Solaris, you must compile with "-D_POSIX_PTHREAD_SEMANTICS" since by
 * default sigwait() uses only the 1st parameter there.
 */

#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int main(void)
{
	char c = 0;
	int fd, pid, sig;
	char *addr;
	sigset_t set;

	if ((fd = open("test.dat", O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1)
		err(1, "open");

	/* extend the file */
	if (write(fd, &c, 1) != 1)
		err(1, "write");

	addr = mmap(0, 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((void *)addr == MAP_FAILED)
		err(1, "mmap");

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_SETMASK, &set, NULL);

	switch (pid = fork()) {
		case -1:
			perror("fork");
			exit(1);
		case 0:
			printf("%d: this is child\n", getpid());
			sigwait(&set, &sig);
			printf("%d: got '%c'\n", getpid(), addr[0]);
			break;
		default:
			printf("%d: this is father\n", getpid());
			printf("type anything (only 1st char matters)...\n");
			if (read(0, &c, 1) != 1)
				err(1, "read");
			addr[0] = c;
			kill(pid, SIGUSR1);
	}

	munmap(addr, 1);
	close(fd);
	return (0);
}
