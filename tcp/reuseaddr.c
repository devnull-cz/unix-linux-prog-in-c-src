/*
 * Demonstrate that repeated bind can result in EADDRINUSE error.
 * To see this error:
 *
 * 1. run the server
 *    ./a.out 0 <port>
 * 2. connect
 *    nc <addr> <port>
 * 3. terminate the server (e.g. with Ctrl-C) and run it again
 *    ./a.out 0 <port>
 *
 * Try this again, this time with first argument set to 1.
 *
 * Note: this example is IPv4 specific.
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	char buf[100];
	int fd, newsock, n;
	struct sockaddr_in sa;
	int optval = 1;
	int reuse;

	if (argc != 3)
		errx(1, "usage: %s <0|1> <port_number>", argv[0]);

	reuse = atoi(argv[1]);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(atoi(argv[2]));
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	if (reuse && (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
	    &optval, sizeof (optval)) == -1))
		err(1, "setsockopt");

	if (bind(fd, (struct sockaddr *) &sa, sizeof (sa)) == -1)
		err(1, "bind");
	if (listen(fd, SOMAXCONN) == -1)
		err(1, "listen");

	for (;;) {
		if ((newsock = accept(fd, NULL, 0)) == -1)
			err(1, "accept");
		while ((n = read(newsock, buf, 100)) != 0)
			write(1, buf, n);
		(void) close(newsock);
		fprintf(stderr, "-- connection closed --\n");
	}

	return (0);
}
