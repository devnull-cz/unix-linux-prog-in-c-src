/*
 * Example on how NOT to resolve IP address to hostnames. Also, this program
 * is IPv4 specific.
 *
 * gethostbyaddr() are int_aton() are obsolete and non standard.
 * Instead use getnameinfo() and inet_pton().
 *
 * This program is included solely as a demonstration of historic APIs.
 */

#define	_DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

int
main(int argc, char **argv)
{
	int i;
	struct in_addr ip;
	struct hostent *hp;

	if (argc != 2)
		errx(1, "usage: %s <ip_address>", argv[0]);

	inet_aton(argv[1], &ip);
	hp = gethostbyaddr((char *) &ip, sizeof (ip), AF_INET);

	if (hp == NULL) {
		herror("gethostbyaddr");
		exit(1);
	}

	/* Print the hostname corresponding to this IP address. */
	printf("%s\n", hp->h_name);

	i = 0;
	/* Print the aliases. */
	while (*(hp->h_aliases + i))
		printf("%s\n", *(hp->h_aliases + i));

	return (0);
}
