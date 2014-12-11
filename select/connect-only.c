/*
 * Connect only. To show what happens if the remote side starts writing but we
 * do not read. Use with select/write-select.c.
 *
 * Instead of this program you can easily use netcat(1). For example:
 *
 *	nc -i 9999 localhost 2222
 *
 * Note that this program works with IPv4 only which is bad.
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <strings.h>

int
main(int argc, char **argv)
{
	int fd;
	struct sockaddr_in in;

	if (argc != 3)
		errx(1, "usage: %s <ip-address> <port>", argv[0]);

	bzero(&in, sizeof (in));
	in.sin_family = AF_INET;
	in.sin_port = htons(atoi(argv[2]));

	/*
	 * Convert a dotted format to 4 bytes suitable for use
	 * in sockaddr_in.
	 */
	if (inet_pton(AF_INET, argv[1], &(in.sin_addr)) != 1)
		errx(1, "inet_pton failed for '%s'", argv[1]);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	if (connect(fd, (struct sockaddr *) &in, sizeof (in)) == -1)
		err(1, "connect");

	/* Wait for ever. */
	while (1)
		pause();

	/* Not reached. */
	close(fd);
	return (0);
}
