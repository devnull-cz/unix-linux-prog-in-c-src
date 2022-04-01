/*
 * Print all strings from argv[].
 *
 * You can do it multiple ways.
 */
#include <stdio.h>

int
main(int argc, char **argv)
{
	int i = 0;

	printf("argv guided:\n");
	while (argv[i] != NULL) {
		printf("[%d] %s\n", i, argv[i]);
		++i;
	}

	printf("argc guided:\n");
	for (i = 0; i < argc; ++i)
		printf("[%d] %s\n", i, argv[i]);

	/*
	 * And, as the last one which keeps the state in argv, we can even do
	 * the following.
	 */
	printf("argv guided simplified:\n");
	while (*argv != NULL)
		printf("%s\n", *argv++);
}
