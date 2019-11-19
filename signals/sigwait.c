/*
 * Two processes share a piece of memory and synchronize with SIGUSR1.  When the
 * parent reads a character from the terminal, it puts it to the shared memory
 * and sends a signal to its child.
 *
 * The child is waiting in sigwait().  When the signal is delivered  it reads
 * the memory and prints the character to stdout.  Note that we use sigwait()
 * which does not use handlers at all.
 *
 * On Solaris, you must compile with "-D_POSIX_PTHREAD_SEMANTICS" since by
 * default sigwait() uses only the 1st parameter there.
 */

#define	_XOPEN_SOURCE	700

#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
	char c;
	char *addr;
	sigset_t set;
	int fd, pid, sig;

	if ((fd = open("test.dat", O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1)
		err(1, "open");

	/* Extend the file. */
	if (write(fd, "", 1) != 1)
		err(1, "write");

	addr = mmap(0, 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
		err(1, "mmap");

	switch (pid = fork()) {
	case -1:
		err(1, "fork");
	case 0:
		printf("%d: this is a child\n", getpid());
		sigemptyset(&set);
		sigaddset(&set, SIGUSR1);
		sigprocmask(SIG_SETMASK, &set, NULL);
		sigwait(&set, &sig);
		assert(sig == SIGUSR1);
		printf("%d: got '%c'\n", getpid(), addr[0]);
		break;
	default:
		printf("%d: this is a parent\n", getpid());
		printf("%d: type anything (only the 1st char matters)...\n",
		    getpid());
		if (read(0, &c, 1) != 1)
			err(1, "read");
		addr[0] = c;
		printf("%d: sending a signal to %d\n", getpid(), pid);
		kill(pid, SIGUSR1);
		wait(NULL);
	}

	munmap(addr, 1);
	close(fd);
}
