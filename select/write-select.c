/*
 * Example on 
 * (c) jp@devnull.cz
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>

#define	BUF_LEN	1024

int
main(int argc, char **argv)
{
	fd_set wrfds;
	char buf[BUF_LEN];
	struct sockaddr_in sa;
	int s, newsock, n, total = 0, optval = 1;

	if (argc != 2)
		errx(1, "usage: %s <port>", argv[0]);

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	sa.sin_family = AF_INET;
	sa.sin_port = htons((short) atoi(argv[1]));
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(buf, '.', BUF_LEN);
	
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) == -1)
		err(1, "bind");

	if (listen(s, SOMAXCONN) == -1)
		err(1, "listen");

	if ((newsock = accept(s, NULL, 0)) == -1)
		err(1, "accept");

	/*
	 * Just write data to the remote side. If the remote side does not read
	 * the data we should be finally put to sleep in select().
	 */
	for ( ; ; ) {
		/* Must do this each time before calling select(). */
		FD_ZERO(&wrfds);
		FD_SET(newsock, &wrfds);
		if (select(newsock + 1, NULL, &wrfds, NULL, NULL) == -1)
			err(1, "select");

		if (FD_ISSET(newsock, &wrfds)) {
			if ((n = write(newsock, buf, BUF_LEN)) == -1)
				err(1, "write (%d)", errno);
			total = total + n;
			fprintf(stderr, "[ %d bytes written (total %d) ]", n, total);
		}
	}

	close(newsock);
	close(s);
	return (0);
}
