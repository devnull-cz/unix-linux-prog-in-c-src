/*
 * This define is used to request the implementation make available the symbols
 * and prototypes of Single UNIX Specification V3.
 */
#define	_XOPEN_SOURCE	600

#include <stdio.h>

int
main(void)
{

#if _XOPEN_VERSION == 600
	printf("ok\n");
#else
	printf("no way...\n");
	return (1);
#endif

	return (0);

}
