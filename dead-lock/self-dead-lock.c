/*
 * The program reads from a pipe whose write file descriptor is still open by
 * the same process.  The read operation is put to slep for ever.
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int
main(void)
{
	int c, fd;

	(void) mkfifo("/tmp/test", 0666);

	fd = open("/tmp/test", O_RDWR);
	(void) read(fd, &c, sizeof (c));
	/* never reached */
	return (0);
}
