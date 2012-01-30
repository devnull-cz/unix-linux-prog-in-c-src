#include <stdio.h>

int
main(void)
{
	char *c;
	int n;
	int i = 0;

	c = (char *) &n;

	while (1) {
		c[i++] = 0;
		fprintf(stderr, "%d ", i);
	}

	return (0);
}
