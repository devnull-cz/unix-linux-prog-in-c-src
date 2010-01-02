/*
 * Will generate a coredump if built with gcc but will not crash if built with
 * Sun Studio, for example. The reason is that gcc puts all static strings into
 * a read-only segment.
 */
int
main(void)
{
	char *c = "hey world";

	c[0] = '\0';

	return (0);
}
