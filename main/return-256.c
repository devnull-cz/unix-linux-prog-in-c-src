/*
 * Shell interprets the return value as a 1 byte integer. So, this must
 * return 0 which might seem unexpected to one who doesn't know...
 *
 *   $ ./a.out
 *   $ echo $?
 *   0
 *
 * (c) jp@devnull.cz
 */

int
main(void)
{
	return (256);
}
