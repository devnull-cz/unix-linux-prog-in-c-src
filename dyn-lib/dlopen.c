/*
 * A simple program to show how dlopen/dlsym functions work. You need libmy.c
 * which should be in the same directory as this file.  Build it like this:
 *
 * Solaris:
 *  $ cc -o libmy.so -KPIC -G libmy.c
 *  $ cc dlopen.c
 *
 * Mac OS X:
 *  $ gcc -fPIC -dynamiclib -Wl,-undefined,dynamic_lookup -o libmy.so libmy.c
 *  $ gcc dlopen.c
 *
 * Linux:
 *  $ gcc -shared -fPIC -o libmy.so libmy.c
 *  $ gcc -ldl dlopen.c
 *
 * If run with the "now" option, the program fails without printing anything as
 * the dynamic linker tries to resolve all the symbols upon execution and bails
 * out on foo() right away.  With the "lazy" option, the libmy's myprint()
 * function is executed, printing "hello", and not until then the program exits
 * on calling foo().
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <dlfcn.h>

int
main(int argc, char *argv[])
{
	fn f;
	int flag;
	void *handle;
	typedef void (*fn)(void);

	if (argc != 2)
		errx(1, "usage: %s <now|lazy>", argv[0]);

	if (strcmp(argv[1], "now") == 0)
		flag = RTLD_NOW;
	else if (strcmp(argv[1], "lazy") == 0)
		flag = RTLD_LAZY;
	else
		errx(1, "invalid flag value");

	if ((handle = dlopen("./libmy.so", flag)) == NULL)
		errx(1, "dlopen: %s", dlerror());

	if ((f = (fn) dlsym(handle, "myprint")) == NULL)
		errx(1, "dlsym: %s", dlerror());

	/* Now let's call the function. */
	f();

	return (0);
}
