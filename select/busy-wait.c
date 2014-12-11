/*
 * Example on busy waiting. The purpose of this is to show what we could do
 * without select() or poll(). The program accepts TCP connection on port from
 * the 1st parameter and reads the standard input as well. See select/select.c
 * about how to this properly.
 *
 * Note that the server is single threaded and always finishes the TCP
 * connection so it does not read from the terminal until the connection
 * terminates.
 *
 * Busy waiting is bad and since we have ways on how not to do that, do not use
 * it. You will NOT get away with that at your exam.
 *
 * Note: this example is IPv4 specific which is also bad.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

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
	sa.sin_port = htons((short)atoi(argv[1]));
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval,
	    sizeof (optval)) == -1)
		err(1, "setsockopt");

	if (bind(fd, (struct sockaddr *) &sa, sizeof (sa)) == -1)
		err(1, "bind");

	if (listen(fd, SOMAXCONN) == -1)
		err(1, "listen");

	/*
	 * Set stdout and the listener socket to a non-blocking mode.
	 * Notice that we are using F_SETFL, not F_SETFD.
	 */
	if (fcntl(1, F_SETFL, O_NONBLOCK) == -1)
		err(1, "fcntl");
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		err(1, "fcntl");

	for (;;) {
		/*
		 * We set fd 1 to a non-blocking mode above, not 0. Why does it
		 * still work as expected ?
		 */
		if ((n = read(0, buf, sizeof (buf))) >= 0) {
			if (n == 0) {
				/* EOF */
				break;
			}
			write(1, buf, n);
		} else {
			if (errno != EWOULDBLOCK)
				err(1, "read");
		}

		if ((newsock = accept(fd, NULL, 0)) == -1) {
			if (errno != EWOULDBLOCK)
				err(1, "accept");
		} else {
			int flags;

			/* Set the newsock back to a blocking mode. */
			flags = fcntl(newsock, F_GETFL);
			fcntl(newsock, F_SETFL, flags & ~O_NONBLOCK);

			fprintf(stderr, "-- connection accepted --\n");
			/*
			 * Note that newsock could be added to the select RD set
			 * and data could be then read from fd 0 even during the
			 * socket connection. It is left as an excercise for
			 * you.
			 */
			while ((n = read(newsock, buf, BUF_LEN)) > 0)
				write(1, buf, n);
			if (n == -1)
				err(1, "read");
			close(newsock);
			fprintf(stderr, "-- connection closed --\n");
		}

		/* Wait a little bit before the next loop. */
		fprintf(stderr, "-- waiting --\n");
		sleep(2);
	}

	return (0);
}
