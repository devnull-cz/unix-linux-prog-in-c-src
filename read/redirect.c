/*
 * Implement the following command:
 *
 *	cat <argv1 >argv2
 *
 * It means that after running "./a.out" the program will copy the existing file
 * in argv[1] to a newly created (or an existing truncated) file argv[2].
 *
 * Run like this:
 *
 *	cc redirect.c
 *	./a.out /etc/passwd out
 *	diff /etc/passwd out		# should not print any difference
 */
#include <sys/stat.h>
#include <sys/types.h>

#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/* Assumes that the standard descriptors are present. */
int
main(int argc, char **argv)
{
	int fd;

	assert(argc == 3);

	if ((fd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open");

	/* Do the "<argv1" part. */
	close(0);
	dup(fd);
	/* Always close what you no longer need. */
	close(fd);

	/* Do the ">argv2" part. */
	close(1);
	if (open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666) == -1)
		err(1, "open");

	/* No need for "if".  If we get past this one we know execl() failed. */
	execl("/bin/cat", "cat", NULL);
	err(1, "execl");
}
