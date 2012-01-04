/*
 * Example of getnameinfo(3socket) usage.
 *
 * www.ipv6.cz is an alias for staff.cesnet.cz which has A and AAAA DNS record:
 *
 *   $ ./a.out www.ipv6.cz
 *   host 195.113.144.241 resolves to staff.cesnet.cz
 *   host 2001:718:1:101::6 resolves to staff.cesnet.cz
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <string.h>
#include <err.h>
#include <libgen.h>
#include <sys/socket.h>
#include <netdb.h>

int
main(int argc, char **argv)
{
	int error;
	char host[NI_MAXHOST], numhost[NI_MAXHOST];
	struct addrinfo *res, *resorig, hints;

	if (argc != 2)
		errx(1, "usage: %s <hostname>", basename(argv[0]));

	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/*
	 * Get our sockaddr structure(s) first via getaddrinfo().
	 * We do not care about port number.
	 */
	if ((error = getaddrinfo(argv[1], "0", &hints, &res)) != 0)
		errx(1, "%s", gai_strerror(error));

	for (resorig = res; res != NULL; res = res->ai_next) {
		/* Here getnameinfo() is used ala inet_ntop(). */
		if ((error = getnameinfo(res->ai_addr, res->ai_addrlen,
		    numhost, sizeof (numhost), NULL, 0, NI_NUMERICHOST)) != 0)
			errx(1, "%s", gai_strerror(error));
		/* Now try to resolve the address back to hostname. */
		if ((error = getnameinfo(res->ai_addr, res->ai_addrlen,
		    host, sizeof (host), NULL, 0, 0)) != 0)
			errx(1, "%s", gai_strerror(error));

		printf("host %s resolves to %s\n", numhost, host);
	}

	freeaddrinfo(resorig);

	return (0);
}
