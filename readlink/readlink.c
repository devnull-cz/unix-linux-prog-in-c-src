#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <err.h>


#define	SYMLINK	"foobar"

int
main(int argc, char *argv[])
{
	char buf[PATH_MAX];

	(void) memset(buf, 'A', sizeof (buf));
	buf[sizeof (buf) - 1] = '\0';

	if (unlink(SYMLINK) != 0)
		err(1, "unlink");

	if (symlink("/etc/blah", SYMLINK) == -1)
		err(1, "symlink");

	if (readlink(SYMLINK, buf, sizeof (buf)) == -1)
		err(1, "readlink");

	printf("%s\n", buf);

	return (0);
}
