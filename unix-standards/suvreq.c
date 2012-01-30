/*
 * Asking the system what it has
 * AUP2, Sec. 1.05.3
 *
 * Copyright 2003 by Marc J. Rochkind. All rights reserved.
 * May be copied only for purposes and under conditions described
 * on the Web page www.basepath.com/aup/copyright.htm.
 *
 * The Example Files are provided "as is," without any warranty;
 * without even the implied warranty of merchantability or fitness
 * for a particular purpose. The author and his publisher are not
 * responsible for any damages, direct or incidental, resulting
 * from the use or non-use of these Example Files.
 *
 * The Example Files may contain defects, and some contain deliberate
 * coding mistakes that were included for educational reasons.
 * You are responsible for determining if and how the Example Files
 * are to be used.
 */

#define	SUV_SUS2

#if 1
#include "suvreq.h"
#endif
#include <stdio.h>
#include <unistd.h>
#include <err.h>

int
main(void)
{
	printf("Request:\n");

#ifdef _POSIX_SOURCE
	printf("\t_POSIX_SOURCE defined\n");
	printf("\t_POSIX_C_SOURCE = %ld\n", (long)_POSIX_C_SOURCE);
#else
	printf("\t_POSIX_SOURCE undefined\n");
#endif

#ifdef _XOPEN_SOURCE
#if _XOPEN_SOURCE == 0
	printf("\t_XOPEN_SOURCE defined (0 or no value)\n");
#else
	printf("\t_XOPEN_SOURCE = %d\n", _XOPEN_SOURCE);
#endif
#else
	printf("\t_XOPEN_SOURCE undefined\n");
#endif

#ifdef _XOPEN_SOURCE_EXTENDED
	printf("\t_XOPEN_SOURCE_EXTENDED defined\n");
#else
	printf("\t_XOPEN_SOURCE_EXTENDED undefined\n");
#endif

	printf("Claims:\n");
#ifdef _POSIX_VERSION
	printf("\t_POSIX_VERSION = %ld\n", (long)_POSIX_VERSION);
#else
	printf("\tNot POSIX\n");
#endif

#ifdef _XOPEN_UNIX
	printf("\tX/Open\n");
#ifdef _XOPEN_VERSION
	printf("\t_XOPEN_VERSION = %d\n", _XOPEN_VERSION);
#else
	printf("\tError: _XOPEN_UNIX defined, but not _XOPEN_VERSION\n");
#endif
#else
	printf("\tNot X/Open\n");
#endif

	return (0);
}
