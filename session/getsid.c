#define	_XOPEN_SOURCE	700	// for getsid()

#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	printf("session id: %d\n", getsid(0));
	return (0);
}
