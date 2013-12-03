/*
 * UDP server. Read data from the net and print them onto stdout.
 *
 * Note: this example is IPv4-only !
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <err.h>

#define	BUF_LEN	1024

int
main(int argc, char **argv)
{
	char buf[BUF_LEN + 1];
	int s, n;
	struct sockaddr_in in, faddr;
	int addrsize = sizeof (faddr);

	if (argc != 2)
		errx(1, "usage: %s <port>", argv[0]);

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		err(1, "socket");

	in.sin_family = AF_INET;
	in.sin_port = htons(atoi(argv[1]));
	in.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *) &in, sizeof (in)) == -1)
		err(1, "bind");

	while ((n = recvfrom(s, buf, BUF_LEN, 0, (struct sockaddr *)&faddr,
	    &addrsize)) > 0) {
		fprintf(stderr, "DBG: received %d bytes\n", n);
		buf[n] = '\0';
		printf("%s", buf);
	}

	if (n == -1)
		err(1, "recvfrom");

	close(s);
	return (0);
}
