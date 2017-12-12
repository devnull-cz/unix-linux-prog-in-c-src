/*
 * Demonstrate the use of getservbyname() and getprotobyname().
 *
 * Note: this example is IPv4 only !
 */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int
main(int argc, char **argv)
{
	struct servent *sp;
	struct protoent *pp;
	struct sockaddr_in sa;
	char buf[100];
	int fd, newsock, n;

	if (argc != 3)
		errx(1, "usage: %s <service> <protocol_name>", argv[0]);

	/* get the protocol entry first as we will need it for service lookup */
	pp = getprotobyname(argv[2]);
	if (pp == NULL)
		errx(1, "'%s' not valid protocol name", argv[2]);

	/* now lookup service for the protocol */
	sp = getservbyname(argv[1], argv[2]);
	if (sp == NULL) {
		printf("'%s' not a known service name, trying port number\n",
		    argv[1]);
		sp = getservbyport(htons(atoi(argv[1])), argv[2]);
		if (sp == NULL)
			errx(1, "not valid service port number either");
	}

	printf("official service name: %s\n", sp->s_name);
	if ((fd = socket(AF_INET, SOCK_STREAM, pp->p_proto)) == -1)
		err(1, "socket");

	sa.sin_family = AF_INET;
	/* Do NOT use htons() here, it's already in network byte order ! */
	sa.sin_port = sp->s_port;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(fd, (struct sockaddr *)&sa, sizeof (sa)) == -1)
		err(1, "bind");

	if (listen(fd, SOMAXCONN) == -1)
		err(1, "listen");

	for (;;) {
		printf("waiting for a connection\n");
		newsock = accept(fd, NULL, 0);
		while ((n = read(newsock, buf, sizeof (buf))) > 0)
			write(1, buf, n);
		close(newsock);
		fprintf(stderr, "-- connection closed --\n");
	}

	return (0);
}
