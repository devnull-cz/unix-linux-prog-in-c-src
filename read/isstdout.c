/*
 * See if there is a way how to determine if file descriptor points to stdout.
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void
statfd(int fd)
{
	struct stat st;

	if (fstat(fd, &st) == -1)
		err(1, "fstat");
	printf("fd %d (tty: %s)\n", fd, isatty(fd) ? ttyname(fd) : "no");
	printf("  dev = %d/%d\n", major(st.st_dev), minor(st.st_dev));
	printf("  inode = %lu\n", (long) st.st_ino);
}

int
main(void)
{
	int flags;

	printf("fileno(stdout) = %d\n", fileno(stdout));
	printf("fileno(stderr) = %d\n", fileno(stderr));
	printf("before dup\n");
	if (dup2(2, 1) == -1)
		err(1, "dup2");
	printf("after dup\n");
	fprintf(stdout, "std\n");

	statfd(1);
	statfd(2);
}
