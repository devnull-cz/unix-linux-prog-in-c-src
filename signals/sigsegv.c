/*
 * Segmentation violation example.
 */

#include <stdio.h>

int
main(void)
{
	int *p = NULL;

	*p = 1;

	return (0);
}
