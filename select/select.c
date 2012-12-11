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

#include <stdio.h>
#include <strings.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

#define	BUF_LEN	100

int
main(int argc, char **argv)
{
	fd_set rdfds;
	char buf[BUF_LEN];
	struct sockaddr_in sa;
	int s, newsock, n, optval = 1;

	if (argc != 2)
		errx(1, "usage: %s <port>", argv[0]);

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	sa.sin_family = AF_INET;
	sa.sin_port = htons((short) atoi(argv[1]));
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));

	if (bind(s, (struct sockaddr *) &sa, sizeof (sa)) == -1)
		err(1, "bind");

	if (listen(s, SOMAXCONN) == -1)
		err(1, "listen");

	for (;;) {
		/* Must do this each time before calling select(). */
		FD_ZERO(&rdfds);
		FD_SET(0, &rdfds);
		FD_SET(s, &rdfds);
		if (select(s + 1, &rdfds, NULL, NULL, NULL) == -1)
			err(1, "select");

		if (FD_ISSET(0, &rdfds)) {
			if ((n = read(0, buf, sizeof (buf))) == 0)
				break;
			if (n == -1)
				err(1, "read");
			write(1, buf, n);
		}

		if (FD_ISSET(s, &rdfds)) {
			newsock = accept(s, NULL, 0);
			fprintf(stderr, "-- connection accepted --\n");
			/*
			 * As with busy-wait.c, the accepted socket could be put
			 * into the RD set and we could accept data from fd 0
			 * while connected. It's left as an excercise to the
			 * reader.
			 */
			while ((n = read(newsock, buf, 100)) > 0)
				write(1, buf, n);
			if (n == -1)
				err(1, "read");
			close(newsock);
			fprintf(stderr, "-- connection closed --\n");
		}
	}

	close(s);
	return (0);
}
