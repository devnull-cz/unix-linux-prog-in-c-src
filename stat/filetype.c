/*
 * Show the file type as a number. Print out the most used file types - regular
 * file and directory.
 */
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int
main(int argc, char **argv)
{
	struct stat buf;

	if (argc != 2)
		errx(1, "Filename/dir missing.");

	(void) lstat(argv[1], &buf);
	printf("Filetype for '%s': 0x%x\n", argv[1], buf.st_mode & S_IFMT);
	printf("S_IFREG is: 0x%x\n", S_IFREG);
	printf("S_IFDIR is: 0x%x\n", S_IFDIR);
	printf("S_IFLNK is: 0x%x\n", S_IFLNK);

/*
	S_ISDIR
	*/

	return (0);
}
