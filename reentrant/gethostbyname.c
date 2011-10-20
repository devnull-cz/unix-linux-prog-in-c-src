/*
 * Demonstrate that gethostbyname() is not safe to use even in
 * single threaded program.
 * 
 * Compile:
 *   gcc -o get gethostbyname.c
 *
 * Run like this:
 *   ./get www.google.com www.opensolaris.org
 *   ./get www.opensolaris.org www.google.com
 *
 * Vladimir Kotal, 2010
 */

#include <stdio.h>
#include <netdb.h>

void
print_hp(const char *comment, struct hostent *hp)
{
	char **p;

	printf("# %s [%p/%p]:\n", comment, hp, hp->h_addr_list);
	for (p = hp->h_addr_list; *p != 0; p++) {
                struct in_addr in;
                (void) memcpy(&in.s_addr, *p, sizeof (in.s_addr));
                (void) printf("%s [%s]\n", inet_ntoa(in), hp->h_name);
	}
}

int
main(int argc, char *argv[]) {
	struct hostent *hp1, *hp2;
	int seq = 0;
	int optint = 1;

	if (argc != 3 && argc != 4) {
		printf("usage: %s [-s] <host1> <host2>\n", argv[0]);
		printf("-s\tprint right after each call to gethostbyname()\n");
		exit(1);
	}

	if (!strcmp(argv[1], "-s")) {
		seq = 1;
		optind++;
	}

	if (!strcmp(argv[optind], argv[optind + 1])) {
		printf("hosts should be different to make the point\n");
		exit(1);
	}

	if (!(hp1 = gethostbyname(argv[optind]))) {
		printf("cannot resolve %s\n", argv[optind]);
		exit(1);
	}

	if (seq)
		print_hp("host1", hp1);

	if (!(hp2 = gethostbyname(argv[optind + 1]))) {
		printf("cannot resolve %s\n", argv[optind + 1]);
		exit(1);
	}

	if (!seq)
		print_hp("host1", hp1);
	print_hp("host2", hp2);

	return (0);
}