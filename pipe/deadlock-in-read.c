/*
 * The program reads from a pipe whose write file descriptor is still open by
 * the same process.  The read operation is put to sleep for ever, deadlocking
 * the process as it is waiting for an event that is never going to happen.
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

	(void) read(fd[0], &c, sizeof (c));
	/* Never reached. */
	return (0);
}
