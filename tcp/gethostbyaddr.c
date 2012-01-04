/*
 * Example on how NOT to resolve IP address to hostnames.
 *
 * Instead of gethostbyaddr() use getaddrinfo() because it is standard
 * function.
 */

#include <stdio.h>
#include <stdlib.h>

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
	hp = gethostbyaddr((char *) &ip, sizeof(ip), AF_INET);

	if (hp == NULL)
		exit(1);

	printf("%s\n", hp->h_name);

	i = 0;
	while (*(hp->h_aliases + i))
		printf("%s\n", *(hp->h_aliases + i));

	return (0);
}
