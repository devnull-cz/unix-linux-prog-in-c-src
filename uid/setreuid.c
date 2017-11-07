#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <err.h>

int
main(void)
{
	printf("real ID:\t%s\n", getpwuid(getuid())->pw_name);
	printf("effective ID:\t%s\n", getpwuid(geteuid())->pw_name);
	printf("real GID:\t%s\n", getgrgid(getgid())->gr_name);
	printf("effective GID:\t%s\n\n", getgrgid(getegid())->gr_name);

	if (open("test", O_RDONLY) == -1) {
		warn("test");
	} else {
		printf("ok, open() succeeded\n");
	}

#if 0
	if (setreuid(1062, 1062) == -1) {
#endif
	if (setreuid(1064, 1062) == -1)
		err(1, "setreuid");

	printf("\nafter setreuid()...\n\n");

	printf("real ID:\t%s\n", getpwuid(getuid())->pw_name);
	printf("effective ID:\t%s\n", getpwuid(geteuid())->pw_name);
	printf("real GID:\t%s\n", getgrgid(getgid())->gr_name);
	printf("effective GID:\t%s\n\n", getgrgid(getegid())->gr_name);

	if (open("test", O_RDONLY) == -1) {
		warn("test");
	} else {
		printf("ok, open() succeeded\n");
	}

	return (0);
}
