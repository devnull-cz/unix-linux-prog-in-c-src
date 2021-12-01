/*
 * The program reads from a pipe whose write file descriptor is still open by
 * the same process.  The read operation is put to sleep for ever, deadlocking
 * the process as it is waiting for an event that is never going to happen.
 *
 * If the program closed fd[1] as it should (as it is not expected to use it),
 * the read(2) call would return 0 as an indication of EOF.
 *
 * (c) jp@devnull.cz
 */

#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int
main(void)
{
	int c, fd[2];

	if (pipe(fd) == -1)
		err(1, "pipe");

#if 0
	close(fd[1]);
#endif
	(void) read(fd[0], &c, sizeof (c));
	/* Never reached. */
	return (0);
}
