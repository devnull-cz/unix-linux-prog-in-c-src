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

	if (argc != 2) {
		fprintf(stderr, "bad input\n");
		return (1);
	}

	sp = getservbyname(argv[1], "tcp");
	pp = getprotobyname("tcp");
	fd = socket(AF_INET, SOCK_STREAM, pp->p_proto);

	sa.sin_family = AF_INET;
	/* do NOT use htons() here ! */
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
