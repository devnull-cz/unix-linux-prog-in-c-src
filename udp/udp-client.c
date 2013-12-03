/*
 * UDP client. Read the standard input and feed the other side.
 *
 * Note: this example is IPv4-only !
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <err.h>

#define	BUF_LEN	1024

int
main(int argc, char **argv)
{
	int s, n;
	char buf[BUF_LEN];
	int have_address = 0;
	struct sockaddr_in taddr;

	if (argc != 3)
		errx(1, "usage: %s <ip-address> <port>", argv[0]);

	bzero(&taddr, sizeof (taddr));

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err(1, "socket");

	taddr.sin_family = AF_INET;
	taddr.sin_port = htons(atoi(argv[2]));
	/* convert a dotted format to 4 bytes suitable for use in sockaddr_in */
	if (inet_pton(AF_INET, argv[1], &(taddr.sin_addr)) != 1)
		errx(1, "inet_pton failed for '%s'", argv[1]);

	/* read the standard input and feed the other side */
	while ((n = read(0, buf, sizeof (buf))) > 0) {
		fprintf(stderr, "DBG: sending %d bytes...\n", n);
		if (sendto(s, buf, n, 0, (struct sockaddr *)&taddr,
		    sizeof (taddr)) == -1) {
			err(1, "sendto");
		}
	}

	if (n == -1)
		err(1, "read");

	close(s);
	return (0);
}
