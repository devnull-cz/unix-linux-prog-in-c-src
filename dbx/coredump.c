/*
 * Will crash and generate a core dump.
 */
void
bad_memory_access(void)
{
	char *x = 0x00;
	x[0] = '\0';
}

int
main(void)
{
	bad_memory_access();
	return (0);
}
