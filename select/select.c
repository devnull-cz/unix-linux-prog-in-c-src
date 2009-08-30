/*
 * select() example. The program accepts TCP connection on port from the 1st
 * parameter and reads the standard input as well. Note that there is no busy
 * waiting.
 *
 * Note that the server always finishes the TCP connection so during that, it
 * doesn't read from the terminal until the connection terminates. You can
 * easily rewrite this program to be truly select()ive...
 *
 * (c) jp@devnull.cz
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <err.h>

#define	BUF_LEN	100

int
main(int argc, char **argv)
{
	fd_set rdfds;
	char buf[BUF_LEN];
	struct sockaddr_in sa;
	int fd, newsock, n, optval = 1;

	if (argc != 2)
		errx(1, "usage: %s <port>", argv[0]);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	sa.sin_family = AF_INET;
	sa.sin_port = htons((short) atoi(argv[1]));
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (bind(fd, (struct sockaddr *) &sa, sizeof(sa)) == -1)
		err(1, "bind");

	if (listen(fd, SOMAXCONN) == -1)
		err(1, "listen");

	for ( ; ; ) {
		/* must do this each time before calling select() */
		FD_ZERO(&rdfds);
		FD_SET(0, &rdfds);
		FD_SET(fd, &rdfds);
		select(fd + 1, &rdfds, NULL, NULL, NULL);

		if (FD_ISSET(0, &rdfds)) {
			n = read(0, buf, sizeof(buf));
			write(1, buf, n);
		}

		if (FD_ISSET(fd, &rdfds)) {
			newsock = accept(fd, NULL, 0);
			fprintf(stderr, "-- connection accepted --\n");
			while ((n = read(newsock, buf, 100)) != 0)
				write(1, buf, n);
			close(newsock);
			fprintf(stderr, "-- connection closed --\n");
		}
	}
	return 0;
}
