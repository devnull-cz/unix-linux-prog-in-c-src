/*
 * Connect only.  To show what happens if the remote side starts writing but we
 * do not read.  Use with select/write-select.c.
 *
 * Instead of this program you can easily use BSD (also macOS) netcat(1).
 * GNU/Linux netcat does not help us here as it uses -i for writing only.
 *
 *	nc -i 2 localhost 2222
 *
 * If the 3rd parameter is used, the program will actually read some data each
 * interval.  Use that if you only have a GNU netcat.
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
	int fd, interval = -1;
	struct sockaddr_in in = { 0 };

	if (argc != 3 && argc != 4)
		errx(1, "usage: %s <ip-address> <port> [interval]", argv[0]);

	in.sin_family = AF_INET;
	in.sin_port = htons(atoi(argv[2]));

	/*
	 * Convert a dotted format to 4 bytes suitable for use
	 * in sockaddr_in.
	 */
	if (inet_pton(AF_INET, argv[1], &(in.sin_addr)) != 1)
		errx(1, "inet_pton failed for '%s'", argv[1]);

	if (argc == 4)
		interval = atoi(argv[3]);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err(1, "socket");

	if (connect(fd, (struct sockaddr *) &in, sizeof (in)) == -1)
		err(1, "connect");

	if (interval == -1) {
		/* Wait for ever. */
		while (1)
			pause();
	}

	while (1) {
		int n;
		char buf[4096];

		sleep(interval);
		n = read(fd, buf, sizeof (buf));
		if (n == -1)
			err(1, "read");
		if (n == 0)
			break;
		(void) write(1, buf, n);
	}
	close(fd);
	return (0);
}
