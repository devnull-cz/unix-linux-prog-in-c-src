/*
 * demonstration on how not to use readlink(3). The example below has a
 * certain error that needs to be fixed.
 */

#define	_XOPEN_SOURCE	600

#include <sys/stat.h>

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define	SYMLINK	"foobar"

int
main(int argc, char *argv[])
{
	struct stat stbuf;
	char buf[PATH_MAX];

	(void) memset(buf, 'A', sizeof (buf) - 1);
	buf[sizeof (buf) - 1] = '\0';

	/* Remove the symlink if it exists. */
	if (lstat(SYMLINK, &stbuf) != -1 && unlink(SYMLINK) != 0)
		err(1, "unlink");

	if (symlink("/etc/blah", SYMLINK) == -1)
		err(1, "symlink");

	if (readlink(SYMLINK, buf, sizeof (buf)) == -1)
		err(1, "readlink");

	/* Obviously, something is not right.  Fix the problem. */
	printf("%s\n", buf);
}
