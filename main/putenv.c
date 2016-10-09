/*
 * Show that using putenv() does not copy data to environ.
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(void)
{
	char foo[16] = "FOO=BAR";

	(void) putenv(foo);
	(void) printf("%s\n", getenv("FOO"));

	(void) strncpy(foo, "FOO=RAB", sizeof (foo));
	(void) printf("%s\n", getenv("FOO"));

	return (0);
}
