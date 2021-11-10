/*
 * If you compile this code and set the resulting binary's SUID bit (chmod u+s
 * a.out), anybody running it will be able to create a file /tmp/abc/123 with
 * you as an owner of "abc" directory and him/her as an owner of "123" file.
 *
 * (c) jp@devnull.cz
 */

#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>

int
main(void)
{
	int ret;

	/*
	 * Let's pretend the directory and the file do not exist and that
	 * nothing can go wrong.
	 */

	/*
	 * After setting the SUID bit, this directory will be created with you
	 * as the owner no matter who is running it.
	 * */
	ret = mkdir("/tmp/abc", 0777);
	assert(ret == 0);

	/*
	 * Make sure the real process owner will have privileges to create a
	 * file in here (remember - after we switch EUID to his/her UID we could
	 * not create files in our directorie unless it is writable by group or
	 * all).
	 */
	ret = chmod("/tmp/abc", 0777);
	assert(ret == 0);

	/* Create the file using the process owner real UID. */
	ret = setuid(getuid());
	assert(ret == 0);
	close(open("/tmp/abc/123", O_RDONLY | O_CREAT, 0666));

	return (0);
}
