/*
 * A simple example using execl(2).
 *
 * (c) jp@devnull.cz
 */

#include <unistd.h>

int
main(void)
{
	execl("/bin/date", "date", (char *)NULL);
	return (0);
}
