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
 * Build:
 *   Solaris (if the gcc version you have does not recognize -KPIC use cc(1)):
 *	$ gcc -m64 -KPIC -shared -o myread.so rtld_next.c
 *   OS X:
 *      $ gcc -fPIC -dynamiclib -Wl,-undefined,dynamic_lookup \
 *          -o myread.dylib rtld_next.c
 *   Linux: (_GNU_SOURCE is needed for RTLD_NEXT to be defined)
 *	$ gcc -m64 -fPIC -D_GNU_SOURCE -shared -o myread.so rtld_next.c -ldl
 *
 * Run:
 *   Solaris(if cat(1) is 32-bit remove "-m64" and use LD_PRELOAD_32):
 *	$ LD_PRELOAD_64=./myread.so cat /etc/passwd
 *   Linux:
 *	$ LD_PRELOAD=./myread.so cat /etc/passwd
 *   OS X:
 *      $ DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=./myread.dylib \
 *          cat /etc/passwd
 *
 * Expected output:
 *
 *	== [ fd 3, buf 414020, nbytes 8192, RET (1254) ] ==
 *	root:x:0:0:Super-User:/root:/usr/bin/bash
 *	daemon:x:1:1::/:
 *	bin:x:2:2::/usr/bin:
 *	...
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

#define	NOT_FOUND	"read not found\n"

static ssize_t (*f)(int, void *, size_t);

ssize_t
read(int fildes, void *buf, size_t nbyte)
{
	ssize_t ret;
	char msg[128];

	/* look up read() the first time we are here */
	if (f == NULL) {
		if ((f = dlsym(RTLD_NEXT, "read")) == NULL) {
			/* better use simple function for printing out */
			(void) write(2, NOT_FOUND, strlen(NOT_FOUND));
			exit(1);
		}
	}

	/* this is the real read() called */
	ret = f(fildes, buf, nbyte);

	/* print some statistics, including the return value */
	(void) snprintf(msg, 128, "== [ fd %d, buf %p, nbytes %zd, "
	    "RET (%ld) ] ==\n", fildes, buf, nbyte, (long)ret);
	/* again, let's not use fprintf() for this, write() is enough here */
	(void) write(2, msg, strlen(msg));

	return (ret);
}
