/*
 * If you compile this code and set the resulting binary's SUID bit (chmod u+s
 * a.out), anybody running it will be able to create a file /tmp/abc/123 with
 * you as an owner of "abc" directory and him/her as an owner of "123" file.
 *
 * (c) jp@devnull.cz
 */

#include <fcntl.h>
#include <unistd.h>

int
main(void)
{
	/*
	 * let's pretend the directory and the file do not exist and that
	 * nothing can go wrong.
	 */

	/* create the directory with you as the owner */
	mkdir("/tmp/abc", 0777);

	/*
	 * make sure the user will have enough privileges to create a file
	 * in here (remember - after we switch EUID to his/her own one we
	 * can't normally create files in somebody else's directories).
	 */
	chmod("/tmp/abc", 0777);

	/*
	 * create the directory with the user who started the program as the
	 * owner.
	 */
	setuid(getuid());
	close(open("/tmp/abc/123", O_RDONLY | O_CREAT, 0666));

	return (0);
}
