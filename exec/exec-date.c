/*
 * A simple example using execl(2).
 *
 * (c) jp@devnull.cz
 */

#include <err.h>
#include <unistd.h>

int
main(void)
{
	if (execl("/bin/date", "date", (char *)NULL) == -1)
		err(1, "execl");

	return (0);
}
