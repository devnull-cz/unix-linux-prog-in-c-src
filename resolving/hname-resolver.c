/*
 * For a given hostname, print a list of IP addresses, both IPv4 and IPv6. Use
 * getaddrinfo() and subsequently inet_ntop(). Note that this code accepts IP
 * addresses, too. IPv6 address may be enclosed in brackets.
 *
 * (c) jp@devnull.cz
 */

#define	_XOPEN_SOURCE	700
#define	_GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <libgen.h>

int
main(int argc, char **argv)
{
	int error;
	char *c, *s, *s2;
	char dst[INET6_ADDRSTRLEN];
	struct addrinfo *res, *resorig, hints;

	if (argc != 2)
		errx(1, "usage: %s <hostname>", basename(argv[0]));

	(void) memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((s = s2 = strdup(argv[1])) == NULL)
		err(1, "strdup");
	/* Accept [IPv6-address], too */
	if (s2[0] == '[') {
		++s;
		if ((c = strchr(s, ']')) == NULL)
			errx(1, "missing ']'");
		*c = '\0';
	}
	if ((error = getaddrinfo(s, NULL, &hints, &res)) != 0)
		errx(1, "%s", gai_strerror(error));

	for (resorig = res; res != NULL; res = res->ai_next) {
		struct sockaddr_in *in;
		struct sockaddr_in6 *in6;

		/* always on a watch... */
		if (res->ai_family != AF_INET && res->ai_family != AF_INET6)
			continue;
		if (res->ai_family == AF_INET) {
			in = (struct sockaddr_in *)(res->ai_addr);
			if (inet_ntop(res->ai_family, (void *)(&in->sin_addr),
			    dst, INET6_ADDRSTRLEN) == NULL) {
				errx(1, "inet_ntop");
			}
		} else {
			in6 = (struct sockaddr_in6 *)(res->ai_addr);
			if (inet_ntop(res->ai_family, (void *)(&in6->sin6_addr),
			    dst, INET6_ADDRSTRLEN) == NULL) {
				errx(1, "inet_ntop");
			}
		}
		(void) printf("%s\n", dst);
	}
	freeaddrinfo(resorig);
	return (0);
}
