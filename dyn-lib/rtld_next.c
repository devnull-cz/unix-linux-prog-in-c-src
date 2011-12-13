/*
 * This is a simple example on how RTLD_NEXT handle works. Our objective is to
 * print some debug information onto stderr about every read() called. With
 * RTLD_NEXT we can easily do that. We create a shared object with our read()
 * function, LD_PRELOAD it and print that debug info inside. And we look up and
 * use read() with RTLD_NEXT to do the real work for us. Usually it will be
 * found in libc. Note that no dlopen() is needed here since we already have the
 * handle - RTLD_NEXT.
 *
 * Use like this:
 *
 * Solaris:
 *	$ gcc -shared -o myread.so rtld_next.c
 * Linux:
 *	$ gcc -D_GNU_SOURCE -shared -o myread.so -ldl rtld_next.c
 *
 * Run:
 *	$ LD_PRELOAD=./myread.so cat /etc/passwd
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

#define	NOT_FOUND	"read not found\n"

typedef ssize_t (*fn)(int, void *, size_t);
fn f;

ssize_t
read(int fildes, void *buf, size_t nbyte)
{
	ssize_t ret;
	char msg[128];

	/* look up read() the first time we are here */
	if (f == NULL) {
		if ((f = (fn) dlsym(RTLD_NEXT, "read")) == NULL) {
			/* better use simple function for printing out */
			write(2, NOT_FOUND, strlen(NOT_FOUND));
			exit(1);
		}
	}

	/* this is the real read() called */
	ret = f(fildes, buf, nbyte);

	/* print some statistics, including the return value */
	snprintf(msg, 128, "== [ fd %d, buf %p, nbytes %d, RET (%ld) ] ==\n",
	    fildes, buf, nbyte, (long)ret);
	/* again, let's not use fprintf() for this, write() is enough here */
	write(2, msg, strlen(msg));

	return (ret);
}
