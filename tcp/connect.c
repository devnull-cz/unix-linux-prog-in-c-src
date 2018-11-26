/*
 * Simple TCP client program. Use the 1st command line argument to set the IP
 * address (do not use domain names), and the 2nd one to set the port. Read from
 * the standard input and send the data to the other side. We do not read
 * anything from the remote side.
 *
 * Note: this example is IPv4-only !
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define	BUF_LEN	100

int
main(int argc, char **argv)
{
	int fd, n;
	char buf[BUF_LEN];
	struct sockaddr_in in;

	if (argc != 3)
		errx(1, "usage: %s <ip-address> <port>", argv[0]);

	bzero(&in, sizeof (in));
	in.sin_family = AF_INET;
	in.sin_port = htons(atoi(argv[2]));

	/*
	 * Convert a dotted format to 4 bytes suitable for use in
	 * sockaddr_in.
	 */
	if (inet_pton(AF_INET, argv[1], &(in.sin_addr)) != 1)
		errx(1, "inet_pton failed for '%s'", argv[1]);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	if (connect(fd, (struct sockaddr *)&in, sizeof (in)) == -1)
		err(1, "connect");

	/* Now, read from the standard input and send it to the other side. */
	do {
		if ((n = read(0, buf, 100)) <= 0) {
			close(fd);
			fprintf(stderr, "read finished\n");
			/* Set that we decided to close down. */
			n = 1;
			break;
		}

	} while ((n = write(fd, buf, n)) > 0);

	if (n != 1) {
		fprintf(stderr, "server closed the connection\n");
		close(fd);
	}

	return (0);
}
