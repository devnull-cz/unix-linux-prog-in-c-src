/*
 * Example of getnameinfo(3socket) usage. Get sockaddr structures from the
 * hostname in argv[1] (can be an IP address, too) via getaddrinfo(3cosket) and
 * then for each of those in the addrinfo list, lookup a hostname and print it.
 *
 * Examples:
 *
 * (1) Hostname www.ipv6.cz is an alias for staff.cesnet.cz which has A and AAAA
 * DNS record:
 *
 *	$ ./a.out www.ipv6.cz
 *	host 195.113.144.241 resolves to staff.cesnet.cz
 *	host 2001:718:1:101::6 resolves to staff.cesnet.cz
 *
 * (2) If a loopback interface looks like this (output from Mac OS X 10.6.8):
 *
 *	lo0: flags=8049<UP,LOOPBACK,RUNNING,MULTICAST> mtu 16384
 *		inet6 ::1 prefixlen 128 
 *		inet6 fe80::1%lo0 prefixlen 64 scopeid 0x1 
 *		inet 127.0.0.1 netmask 0xff000000
 *
 * We get the following output:
 *
 *	$ ./a.out localhost
 *	host 127.0.0.1 resolves to localhost
 *	host ::1 resolves to localhost
 *	host fe80::1%lo0 resolves to localhost
 *
 * (c) vlada@devnull.cz, jp@devnull.cz
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
	struct addrinfo *res, *resorig, hints;
	char host[NI_MAXHOST], numhost[NI_MAXHOST];

	if (argc != 2)
		errx(1, "usage: %s <hostname>", basename(argv[0]));

	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/*
	 * Get our sockaddr structure(s) first via getaddrinfo(). We do not
	 * care about a port number so we set "servname" to NULL.
	 */
	if ((error = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
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
