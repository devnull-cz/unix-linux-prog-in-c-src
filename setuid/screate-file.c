/*
 * If you compile this code and set the resulting binary's SUID bit (chmod u+s
 * a.out), anybody running it will be able to create a file /tmp/abc/123 with
 * you as an owner of "abc" directory and him/her as an owner of "123" file.
 *
 * Expected output is for example (on OpenBSD):
 *
 *	RUID: 1001
 *	EUID: 1000
 *	Dir /tmp/abc created.
 *	RUID: 1001
 *	EUID: 1001
 *	File /tmp/abc/123 created.
 *
 * Note that this seems not to work on Linux.  Do SUID binaries only work for
 * root?
 *
 * (c) jp@devnull.cz
 */

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define	ABC	"/tmp/abc"
#define	F123	ABC"/123"

int
main(void)
{
	int ret;

	/*
	 * Let's pretend the directory and the file do not exist and that
	 * nothing can go wrong.
	 */
	printf("RUID: %d\n", getuid());
	printf("EUID: %d\n", geteuid());

	/*
	 * After setting the SUID bit, this directory will be created with you
	 * as the owner no matter who is running it.
	 */
	ret = mkdir(ABC, 0777);
	assert(ret == 0);
	printf("Dir %s created.\n", ABC);

	/*
	 * Make sure the real process owner will have privileges to create a
	 * file in here (remember - after we switch EUID to his/her UID we could
	 * not create files in our directorie unless it is writable by group or
	 * all).
	 */
	ret = chmod(ABC, 0777);
	assert(ret == 0);

	/* Create the file using the process owner real UID. */
	ret = setuid(getuid());
	printf("RUID: %d\n", getuid());
	printf("EUID: %d\n", geteuid());
	assert(ret == 0);
	close(open(F123, O_RDONLY | O_CREAT, 0666));
	printf("File %s created.\n", F123);

	return (0);
}
