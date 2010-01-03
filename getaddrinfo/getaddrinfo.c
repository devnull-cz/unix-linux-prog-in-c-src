/*
 * Example getaddrinfo(3socket) usage. Note that we don't use AI_PASSIVE since
 * we plan to connect to the server and that one must use freeaddrinfo() on
 * exit.
 *
 * As of now, ftp.volny.cz has both IPv4 and IPv6 address:
 *
 *   $ ./a.out ftp.volny.cz ssh
 *   212.20.96.21 port '22' protocol 'ip'
 *   ::2001:1508:1003:4:0:0 port '22' protocol 'ip'
 *
 *   $ ./a.out www.google.com http
 *   address '74.125.87.147' port '80' protocol 'ip'
 *   address '74.125.87.99' port '80' protocol 'ip'
 *   address '74.125.87.103' port '80' protocol 'ip'
 *   address '74.125.87.104' port '80' protocol 'ip'
 *   address '74.125.87.105' port '80' protocol 'ip'
 *
 * (c) jp@devnull.cz
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include <libgen.h>

int
main(int argc, char **argv)
{
	int error;
	struct protoent *proto;
	struct sockaddr_in *sin;
	char addr[INET6_ADDRSTRLEN];
	struct addrinfo *res, *resorig, hints;

	if (argc != 3)
		errx(1, "usage: %s hostname service", basename(argv[0]));

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	/* needed when address structures will be used with bind/listen */
	/* hints.ai_flags = AI_PASSIVE; */

	if ((error = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0)
		errx(1, "%s", gai_strerror(error));

	for (resorig = res; res != NULL; res = res->ai_next) {
		/* ai_addr is of type (struct sockaddr *) */
		sin = (struct sockaddr_in *)res->ai_addr;

		/* always on the watch... */
		if (sin->sin_family != AF_INET && sin->sin_family != AF_INET6)
			continue;

		/* use getprotobynumber_r() in a threaded environment */
		proto = getprotobynumber(res->ai_protocol);
		printf("address '%s' port '%d' protocol '%s'\n",
		    inet_ntop(sin->sin_family, &sin->sin_addr, addr,
		    INET6_ADDRSTRLEN), ntohs(sin->sin_port), proto->p_name);
	}

	freeaddrinfo(resorig);

	return (0);
}
