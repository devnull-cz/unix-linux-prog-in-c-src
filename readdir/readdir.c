/*
 * List contents of directories set as parameters on the command line.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	int i;

	if (argc == 1)
		errx(1, "usage: %s dir1 [dir2 [dir3 ...]]", argv[0]);

	for (i = 1; i < argc; i++) {
		DIR *d;
		struct dirent *de;

		printf("== [%s] ==\n", argv[i]);
		if ((d = opendir(argv[i])) == NULL) {
			warn("%s", argv[i]);
			continue;
		}

		/*
		 * We must distinguish between the end of directory and an
		 * error.
		 */
		errno = 0;
		while ((de = readdir(d)) != NULL)
			printf("%s\n", de->d_name);

		if (errno != 0)
			warn("%s", argv[i]);

		closedir(d);
	}

	return (0);
}
