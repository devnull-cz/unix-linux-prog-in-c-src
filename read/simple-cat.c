#include <assert.h>
#include <unistd.h>

/*
 * Simple cat, reading from 0 and writing to 1.
 *
 * Use to show that redirection is done by shell itself before executing the
 * command.  That is, the following will copy /etc/passwd to output while only
 * using file descriptors 0 and 1.
 *
 * 	$ ./simple-cat </etc/passwd >output
 */
int
main(int argc, char **argv)
{
	int n;
	char buf[128];

	assert(argc == 1);
	(void) argv;

	while ((n = read(0, buf, sizeof (buf))) > 0)
		write(1, buf, n);
}
