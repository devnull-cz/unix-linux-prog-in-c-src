/*
 * Simple TCP sink server running on specified port - what is sent to it is
 * just printed onto stdout. No data is sent back.
 *
 * Note: this example is IPv4-only !
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define	BUF_LEN	100

int
main(int argc, char **argv)
{
	char buf[BUF_LEN];
	struct sockaddr_in in;
	int fd, newsock, n, optval = 1;

	if (argc != 2)
		errx(1, "usage: %s <port_number>", argv[0]);

	bzero(&in, sizeof (in));
	in.sin_family = AF_INET;
	printf("Will use port: %s\n", argv[1]);
	in.sin_port = htons(atoi(argv[1]));
	in.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	/*
	 * So that we can reuse the port immediately. See the lecture materials
	 * for more info.
	 */
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
	    &optval, sizeof (optval)) == -1)
		err(1, "setsockopt");

	if (bind(fd, (struct sockaddr *)&in, sizeof (in)) == -1)
		err(1, "bind");

	if (listen(fd, SOMAXCONN) == -1)
		err(1, "listen");

	/* We will serve one connection at a time. */
	for (;;) {
		if ((newsock = accept(fd, NULL, 0)) == -1)
			err(1, "accept");

		fprintf(stderr, "-- connection accepted --\n");
		while ((n = read(newsock, buf, BUF_LEN)) != 0)
			write(1, buf, n);

		/*
		 * The close() is important cause otherwise you will waste
		 * a few bytes of memory in the kernel with every connection
		 * and eventually run out of file descriptors.
		 */
		(void) close(newsock);
		fprintf(stderr, "-- connection closed --\n");
	}

	return (0);
}
