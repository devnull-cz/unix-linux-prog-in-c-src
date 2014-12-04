/*
 * Example of getaddrinfo(3socket) usage where we try to print the actual
 * converted values back to ascii presentable form.  Normally this use of
 * getnameinfo() is not required since the members of the addrinfo structure can
 * be passed directly to functions such as bind() or connect().  Note that we
 * use NI_NUMERICSERV with getnameinfo() to get numeric address strings.
 *
 * Note that we don't use AI_PASSIVE since we plan to connect to the server
 * and that one must use freeaddrinfo() on exit.
 *
 * www.kame.net maps to both IPv4 and IPv6 address:
 *
 *   $ ./a.out www.kame.net www
 *   address '203.178.141.194' port '80' protocol 'tcp'
 *   address '2001:200:dff:fff1:216:3eff:feb1:44d7' port '80' protocol 'tcp'
 *   $ ./a.out 213.151.78.6 www
 *   address '213.151.78.6' port '80' protocol 'tcp'
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
	char ip_str[INET6_ADDRSTRLEN]; /* NI_MAXHOST would work too */
	char port_str[NI_MAXSERV];
	struct addrinfo *res, *resorig, hints;

	if (argc != 3)
		errx(1, "usage: %s <hostname> <service>", basename(argv[0]));

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
		/*
		 * NI_NUMERICSERV makes sure we get numeric IP address strings
		 * so that we can print those out. Similarly for NI_NUMERICHOST.
		 * Note that when NI_NUMERICHOST is not set, numeric IP address
		 * will be returned if the IP address cannot be located in the
		 * name service.
		 */
		if ((error = getnameinfo(res->ai_addr, res->ai_addrlen,
		    ip_str, sizeof (ip_str), port_str, sizeof (port_str),
		    NI_NUMERICHOST|NI_NUMERICSERV) != 0)) {
			errx(1, "%s", gai_strerror(error));
		}
		printf("address '%s' port '%s' protocol '%s'\n",
		    ip_str, port_str, proto->p_name);
	}

	freeaddrinfo(resorig);

	return (0);
}
