/*
 * A buggy example using execl(2).  It will behave differently on different
 * systems.  On recent Linux kernels, the process is killed by SIGABRT.
 *
 * However, on older systems the 2nd argument is taken as an empty argv[0], and
 * /bin/ls will get whatever is on stack after that.  That could be a list of
 * environment variables, for example (happened on FreeBSD in the past), making
 * it quite confusing:
 * 
 *	$ ./a.out
 *	: BLOCKSIZE=K: No such file or directory
 *	: FTP_PASSIVE_MODE=YES: No such file or directory
 *	: HISTCONTROL=ignoredups: No such file or directory
 *	: HISTSIZE=10000: No such file or directory
 *	...
 *	...
 */

#include <err.h>
#include <unistd.h>

int
main(void)
{
	execl("/bin/ls", NULL);
	err(1, "execl");
}
