/*
 * This define is used to request the implementation make available the symbols
 * and prototypes of Single UNIX Specification V3.
 * It subsumes the definition of the POSIX macro _POSIX_C_SOURCE=200112L
 */
#define	_XOPEN_SOURCE	600

#include <stdio.h>
#include <unistd.h>

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
