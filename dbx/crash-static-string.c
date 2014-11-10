/*
 * Will generate a coredump if built with gcc or LLVM but will not crash if
 * built with Solaris Studio compiler, for example. The reason is that the
 * static string can be put into a read-only segment.
 */

int
main(void)
{
	char *c = "hey world";

	c[0] = '\0';

	return (0);
}
