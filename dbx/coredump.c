/*
 * Will crash and generate a core dump.
 *
 * You might want to run 'ulimit -c size' with reasonable non-zero size to
 * actually get the core file.
 */
void
bad_memory_access(void)
{
	char *x = 0x00;
	x[0] = 'A';
}

int
main(void)
{
	bad_memory_access();
	return (0);
}
