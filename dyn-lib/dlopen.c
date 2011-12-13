/*
 * A simple program to show dlopen/dlsym functions. You need libmy.c which
 * should be in the same directory as this file. You build it like this (don't
 * forget -L/-R options):
 *
 * Solaris:
 *  $ cc -o libmy.so -G libmy.c
 *  $ cc -L. -R. dlopen.c
 *
 * Linux:
 *  $ gcc -shared -o libmy.so libmy.c
 *  $ gcc -L. -Xlinker -R . -ldl dlopen.c
 *
 * (c) jp@devnull.cz
 */

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(void)
{
	typedef void (*fn)(void);
	void *handle;
	fn f;

	if ((handle = dlopen("./libmy.so", RTLD_LAZY)) == NULL)
		errx(1, "dlopen: %s", dlerror());

	if ((f = (fn) dlsym(handle, "myprint")) == NULL)
		errx(1, "dlsym: %s", dlerror());

	/* now let's call the function */
	f();

	return (0);
}
