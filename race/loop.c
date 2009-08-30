#include <stdio.h>

void foo(void)
{
	int a, b, i, j;
	char *addr;

	while (1) {
		a = addr[0];
		b = addr[1];
		if (a != b) {
			fprintf(stderr, "[child	(%d/%d)] ", a, b);
			++j;
		}
		addr[0] = 2;
		addr[1] = 2;
		++i;
	}
}
