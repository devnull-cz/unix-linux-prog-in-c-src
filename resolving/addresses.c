/*
 * Shows the address conversion between the text and binary form, and vice
 * versa. Note that the string can change so we really convert back and forth;
 * note the change in the prefix and the suffix:
 *
 *  $ ./a.out 0:0:2001:1508:1003:4::
 *  a.out: inet_pton failed for '0:0:2001:1508:1003:4::'. Will try INET6 now.
 *  Succesfully converted an IPv6 address string.
 *  Succesfully converted a binary IPv6 address to a string.
 *  The address is: ::2001:1508:1003:4:0:0
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

int
main(int argc, char **argv)
{
	int fd, n;
	char addr[4];
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
		    "    ./a.out 01.01.01.01			# OK", argv[0]);

	if (inet_pton(AF_INET, argv[1], &in) != 1) {
		warnx("inet_pton with AF_INET failed for '%s'. Will try "
		    "INET6 now.", argv[1]);
		if (inet_pton(AF_INET6, argv[1], &in6) != 1)
			errx(1, "inet_pton failed for '%s'.", argv[1]);
		printf("Succesfully converted an IPv6 address string to a "
		    "binary form.\n");
		if (inet_ntop(AF_INET6, &in6, dst, sizeof (dst)) == NULL) {
			err(1, "inet_ntop");
		}
		printf("Succesfully converted a binary IPv6 address "
		    "to a string.\n");
		printf("The address is: %s\n", dst);
	} else {
		printf("Succesfully converted an IPv4 address string to a "
		    "binary form.\n");
		if (inet_ntop(AF_INET, &in, dst, sizeof (dst)) == NULL) {
			err(1, "inet_ntop");
		}
		printf("Succesfully converted a binary IPv4 address "
		    "to a string.\n");
		printf("The address is: %s\n", dst);
	}

	return (0);
}
