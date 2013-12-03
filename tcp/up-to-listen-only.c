/*
 * Start and then connect to given port from another terminal or another
 * machine. You will see that the system initializes the TCP connections even
 * when the program is not in the accept() call, and then accepts some data on
 * it until it reaches the buffer size. After that, it stops accepting more
 * data, waiting for some program to accept() and start reading.
 *
 * Note: this example is IPv4-only !
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libgen.h>

int
main(int argc, char **argv)
{
	struct sockaddr_in in;
	int fd, newsock, n, backlog, optval = 1;

	if (argc < 2)
		errx(1, "usage: %s <port> [backlog]", basename(argv[0]));

	if (argc == 3)
		backlog = atoi(argv[2]);
	else
		backlog = SOMAXCONN;

	printf("using backlog: %d\n", backlog);

	bzero(&in, sizeof (in));
	in.sin_family = AF_INET;
	in.sin_port = htons(atoi(argv[1]));
	in.sin_addr.s_addr = htons(INADDR_ANY);

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
	if (listen(fd, backlog) == -1)
		err(1, "listen");

	/* wait for a signal */
	pause();

	return (0);
}
