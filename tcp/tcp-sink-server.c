/*
 * Simple TCP sink server running on port 2222 - what is sent to it is just
 * printed onto stderr. No data is sent back.
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <err.h>

#define	BUF_LEN	100

int
main(int argc, char **argv)
{
	char buf[BUF_LEN];
	struct sockaddr_in in;
	int fd, newsock, n, optval = 1;

	bzero(&in, sizeof (in));
	in.sin_family = AF_INET;
	in.sin_port = htons(2222);
	in.sin_addr.s_addr = htons(INADDR_ANY);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	/*
	 * So that we can reuse the port immediately. See the lecture materials
	 * for more info.
	 */
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	
	if (bind(fd, (struct sockaddr *)&in, sizeof(in)) == -1)
		err(1, "bind");

	if (listen(fd, SOMAXCONN) == -1)
		err(1, "listen");

	/* we will server one connection at a time */
	for ( ; ; ) {
		if ((newsock = accept(fd, NULL, 0)) == -1)
			err(1, "accept");

		fprintf(stderr, "-- connection accepted --\n");
		while ((n = read(newsock, buf, BUF_LEN)) != 0)
			write(1, buf, n);

		/*
		 * Important since otherwise you will waste a few bytes with
		 * every connection !!!
		 */
		close(newsock);
		fprintf(stderr, "-- connection closed --\n");
	}

	return (0);
}
