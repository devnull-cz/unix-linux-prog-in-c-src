/*
 * Run fstat() on 0.  You will get information on the underlying terminal.  You
 * can check with tty(1) and ls(1) with "-i" to verify the inode number printed
 * by this program.
 *
 * You would get the same information for file descriptors 1 and 2, too.
 */
#include <sys/stat.h>
#include <stdio.h>
#include <inttypes.h>
#include <err.h>

int
main(int argc, char **argv)
{
	struct stat buf;

	if (fstat(0, &buf) == -1)
		err(1, "fstat");

	/*
	 * This should be 0 as there is no data associated with the terminal
	 * device.
	 */
	printf("Allocated number of 512 byte blocks: %jd\n",
	    (intmax_t)buf.st_blocks);
	printf("Inode number: %lu\n", (long) buf.st_ino);

	return (0);
}
