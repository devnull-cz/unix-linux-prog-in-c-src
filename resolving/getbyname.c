/*
 * Demonstrate the use of getservbyname() and getprotobyname().
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

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
		sp = getservbyport(atoi(argv[1]), argv[2]);
		if (sp == NULL)
			errx(1, "not valid service port number either");
	}

	printf("official service name: %s\n", sp->s_name);
	fd = socket(AF_INET, SOCK_STREAM, pp->p_proto);

	sa.sin_family = AF_INET;
	/* do NOT use htons() here, it's already in network byte order ! */
	sa.sin_port = sp->s_port;
	sa.sin_addr.s_addr = INADDR_ANY;

	bind(fd, (struct sockaddr *)&sa, sizeof (sa));
	listen(fd, SOMAXCONN);

	for (;;) {
		newsock = accept(fd, NULL, 0);
		while ((n = read(newsock, buf, 100)) != 0)
			write(1, buf, n);
		close(newsock);
		fprintf(stderr, "-- connection closed --\n");
	}

	return (0);
}
