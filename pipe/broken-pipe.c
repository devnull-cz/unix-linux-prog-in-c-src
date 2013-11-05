/*
 * See how process writing to pipe without readers will get SIGPIPE.
 *
 * Run like this:
 *   ./a.out
 *   ret=$?
 *   echo $ret
 *   kill -l `expr $ret - 128`
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int
main(void)
{
	int pd[2];
	char c = (char) 'a';

	pipe(pd);
	close(pd[0]);
	if (write(pd[1], &c, 1) == -1)
		err(1, "write");

	return (0);
}
