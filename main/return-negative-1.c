/*
 * Shell interprets the return value as a 1 byte integerer. Try this:
 *
 *   $ ./a.out
 *   $ echo $?
 *   255
 *
 * Never use that in your code, it's confusing.
 *
 * (c) jp@devnull.cz
 */

int
main(void)
{
	return (-1);
}
