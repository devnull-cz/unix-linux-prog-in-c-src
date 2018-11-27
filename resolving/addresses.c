/*
 * Shows the address conversion between the text and binary form, and vice
 * versa.  We may get a different string from what we started with while first
 * converting to the binary form.  Note the change in the prefix and the suffix
 * below:
 *
 *	$ ./a.out 0:0:2001:1508:1003:4::
 *	a.out: inet_pton with AF_INET failed for '0:0:2001:1508:1003:4::'.
 *	Will try INET6 now.
 *	Succesfully converted an IPv6 address string.
 *	Succesfully converted a binary IPv6 address to a string.
 *	The address is: ::2001:1508:1003:4:0:0
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <arpa/inet.h>
#include <assert.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int ret;
	struct in_addr in;
	struct in6_addr in6;
	char dst[INET6_ADDRSTRLEN];

	if (argc != 2)
		errx(1, "Usage: %s <ip4-or-ipv6-address>\n"
		    "Examples:\n"
		    "    ./a.out 0:0:2001:1508:1003:4::	# OK\n"
		    "    ./a.out 0:0:0:0:0:0:0:0:1		# bad (18B)\n"
		    "    ./a.out 127.0.0.1			# OK\n"
		    "    ./a.out 10.0.0.299			# bad\n"
		    "    ./a.out 01.01.01.01			# OK/bad, "
		    "it's system dependent", argv[0]);

	if ((ret = inet_pton(AF_INET, argv[1], &in)) != 1) {
		assert(ret == 0);
		warnx("inet_pton with AF_INET failed for '%s'.\nWill try "
		    "INET6 now.", argv[1]);
		if ((ret = inet_pton(AF_INET6, argv[1], &in6)) != 1) {
			assert(ret == 0);
			errx(1, "inet_pton with AF_INET6 failed for '%s'.",
			    argv[1]);
		}
		printf("Succesfully converted an IPv6 address string to a "
		    "binary form.\n");
		if (inet_ntop(AF_INET6, &in6, dst, sizeof (dst)) == NULL) {
			err(1, "inet_ntop");
		}
		printf("Succesfully converted a binary IPv6 address back "
		    "to a string.\n");
		printf("The address is: %s\n", dst);
		return (0);
	}

	printf("Succesfully converted an IPv4 address string to a "
	    "binary form.\n");
	if (inet_ntop(AF_INET, &in, dst, sizeof (dst)) == NULL) {
		err(1, "inet_ntop");
	}
	printf("Succesfully converted a binary IPv4 address back to a "
	    "string.\nThe address is: %s\n", dst);

	return (0);
}
