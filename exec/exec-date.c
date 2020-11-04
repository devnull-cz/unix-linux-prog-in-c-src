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
#if 0
	/*
	 * The "if" control statement is not needed.  If execl() succeeds no
	 * code below it will be executed.
	 */
	if (execl("/bin/date", "date", (char *)NULL) == -1)
#endif
	execl("/bin/date", "date", NULL);
	err(1, "execl");
}
