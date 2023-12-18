/*
 * Demonstrate that gethostbyname() is not safe to use even in single threaded
 * program.  Call it twice on different hostnames and see what happens if we use
 * both pointers to print the output.
 *
 * Compile:
 *   gcc gethostbyname.c
 *
 * Run like this:
 *   ./a.out www.devnull.cz www.mff.cuni.cz
 *   ./a.out www.mff.cuni.cz www.devnull.cz
 *
 * Expected output for the first case:
 *	# host1 [0x7febced31600/0x18f5020]:
 *	195.113.27.222 [wwwmff.karlov.mff.cuni.cz]
 *	# host2 [0x7febced31600/0x18f5020]:
 *	195.113.27.222 [wwwmff.karlov.mff.cuni.cz]
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void
print_hp(const char *comment, struct hostent *hp)
{
	char **p;

	printf("# %s [%p/%p]:\n", comment, (void *)hp, (void *)hp->h_addr_list);
	for (p = hp->h_addr_list; *p != 0; p++) {
		struct in_addr in;

		(void) memcpy(&in.s_addr, *p, sizeof (in.s_addr));
		(void) printf("%s [%s]\n", inet_ntoa(in), hp->h_name);
	}
}

int
main(int argc, char *argv[]) {
	struct hostent *hp1, *hp2;
	int optidx = 0;
	int seq = 0;

	if (argc != 3 && argc != 4) {
		printf("usage: %s [-s] <host1> <host2>\n", argv[0]);
		printf("-s\tprint right after each call to gethostbyname()\n");
		exit(1);
	}

	if (strcmp(argv[1], "-s") == 0) {
		seq = 1;
		optidx++;
	}
	optidx++;

	if (strcmp(argv[optidx], argv[optidx + 1]) == 0) {
		printf("hosts should be different to make the point\n");
		exit(1);
	}

	if (!(hp1 = gethostbyname(argv[optidx]))) {
		printf("cannot resolve %s\n", argv[optidx]);
		exit(1);
	}

	if (seq)
		print_hp("host1", hp1);

	if (!(hp2 = gethostbyname(argv[optidx + 1]))) {
		printf("cannot resolve %s\n", argv[optidx + 1]);
		exit(1);
	}

	if (!seq)
		print_hp("host1", hp1);
	print_hp("host2", hp2);

	return (0);
}
