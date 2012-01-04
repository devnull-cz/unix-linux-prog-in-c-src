/*
 * Example of getaddrinfo(3socket) usage where we try to print the actual
 * converted values back to presentable form. Normally this printing is not
 * required since the members of the addrinfo structure can be passed directly
 * to functions such as bind() or connect().
 *
 * Note that we don't use AI_PASSIVE since we plan to connect to the server
 * and that one must use freeaddrinfo() on exit.
 *
 * www.kame.net maps to both IPv4 and IPv6 address:
 *
 *   $ ./a.out www.kame.net www
 *   address '203.178.141.194' port '80' protocol 'tcp'
 *   address '2001:200:dff:fff1:216:3eff:feb1:44d7' port '80' protocol 'tcp'
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

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
	struct protoent *proto;
	char addr[INET6_ADDRSTRLEN];
	struct addrinfo *res, *resorig, hints;

	if (argc != 3)
		errx(1, "usage: %s hostname service", basename(argv[0]));

	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	/* needed when address structures will be used with bind/listen */
	/* hints.ai_flags = AI_PASSIVE; */

	if ((error = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0)
		errx(1, "%s", gai_strerror(error));

	for (resorig = res; res != NULL; res = res->ai_next) {
		/* always on the watch... */
		if (res->ai_family != AF_INET && res->ai_family != AF_INET6)
			continue;

		/* use getprotobynumber_r() in a threaded environment */
		proto = getprotobynumber(res->ai_protocol);
		printf("address '%s' port '%d' protocol '%s'\n",
		    res->ai_family == AF_INET ?
		    inet_ntop(res->ai_family,
		    &((struct sockaddr_in *)(res->ai_addr))->sin_addr,
		    addr, sizeof (addr)) :
		    inet_ntop(res->ai_family,
		    &((struct sockaddr_in6 *)(res->ai_addr))->sin6_addr,
		    addr, sizeof (addr)),
		    htons(res->ai_family == AF_INET ?
		    ((struct sockaddr_in *)(res->ai_addr))->sin_port :
		    ((struct sockaddr_in6 *)(res->ai_addr))->sin6_port),
		    proto->p_name);
	}

	freeaddrinfo(resorig);

	return (0);
}
