/*
 * Example on how to work with a non-blocking connect. Uses fixed input and
 * should show all 3 situations we care about - a successful connect, a refused
 * connect, and a timeout. Uses legacy functions gethostbyname() and
 * inet_ntoa(). Use getaddrinfo() and inet_ntop() to make this work with IPv6 as
 * well.
 * 
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define	TIMEOUT 10
#define	MAX_HOSTS 5

int
main(int argc, char **argv)
{
	socklen_t optlen;	
	struct hostent *he;
	struct timeval tout;
	struct sockaddr_in in;
	fd_set wrfds, wrfds_orig;
	char *hostnames[MAX_HOSTS];
	int flags, i, optval, n, timeouts;
	int sockets[MAX_HOSTS], ports[MAX_HOSTS];

	/* This should connect OK. */
	hostnames[0] = "www.devnull.cz";
	ports[0] = 80;
	hostnames[1] = "www.devnull.cz";
	/* This will timeout. */
	ports[1] = 33;
	/* This should refuse the connection. */
	hostnames[2] = "mail.kolej.mff.cuni.cz";
	ports[2] = 999;
	/* To see if localhost gets EINPROGRESS as well. */
	hostnames[3] = "localhost";
	ports[3] = 22;
	/*
	 * To see if localhost gets EINPROGRESS as well. Connection refusal
	 * might be different from an open port.
	 */
	hostnames[4] = "localhost";
	/* Port 7 is for echo, nobody should run that service these days. */
	ports[4] = 7;

	printf("will work with those machines/services:\n");
	for (i = 0; i < MAX_HOSTS; ++i) {
		printf("  %s:%d\n", hostnames[i], ports[i]);
	}

	printf("making all sockets non-blocking, and calling connect...\n");
	FD_ZERO(&wrfds);
	/* counts finished connects */
	n = 0;
	for (i = 0; i < MAX_HOSTS; ++i) {

		if ((he = gethostbyname(hostnames[i])) == NULL)
			errx(1, "gethostbyname error");

		bzero(&in, sizeof (in));
		in.sin_family = AF_INET;
		in.sin_port = htons(ports[i]);
		in.sin_addr = (*((struct in_addr *)he->h_addr_list[0]));

		printf("  we got this IP address for %s: ", hostnames[i]);
		printf("%s\n", inet_ntoa(*((struct in_addr *)he->h_addr_list[0])));

		if ((sockets[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			err(1, "socket");

		flags = fcntl(sockets[i], F_GETFL, NULL);
		if (fcntl(sockets[i], F_SETFL, flags | O_NONBLOCK) == -1)
			err(1, "fcntl");

		if (connect(sockets[i], (struct sockaddr *)&in,
		    sizeof(in)) == -1) {
			/* this is what we expect */
			if (errno == EINPROGRESS) {
				printf("    connect EINPROGRESS OK "
				    "(expected)\n");
				FD_SET(sockets[i], &wrfds);
			} else {
				/*
				 * This may happen right here, on localhost
				 * for example (immediate connection refused).
				 * I can see that happen on FreeBSD but not on
				 * Solaris, for example.
				 */
				printf("    connect: %s\n", strerror(errno));
				++n;
			}
		}
		else {
			/* this may happen, on localhost for example */
			printf("  %s connected OK on %d\n", hostnames[i],
			    ports[i]);
			++n;
		}
	}

	/* save our original fd set */
	wrfds_orig = wrfds;
	/* let's print some progress every seconds */
	tout.tv_sec = 1;
	tout.tv_usec = 0;

	printf("continuing with select now, checking the connect status...\n");
	timeouts = 0;
	while (n != MAX_HOSTS) {
		/*
		 * Note that non-blocking connect uses the WR set, not the RD
		 * one.
		 */
		i = select(FD_SETSIZE, NULL, &wrfds, NULL, &tout);

		if (i == 0) {
			if (++timeouts == TIMEOUT) {
				printf("  TIMEOUT REACHED\n");
				break;
			}
			printf("  .\n");
			/*
			 * select may change the timeval structure (see the
			 * spec)
			 */
			tout.tv_sec = 1;
			tout.tv_usec = 0;
			continue;
		} else {
			if (i == -1)
				err(1, "select");
		}

		for (i = 0; i < MAX_HOSTS; ++i) {
			if (FD_ISSET(sockets[i], &wrfds)) {
				optval = -1;
				optlen = sizeof(optval);

				if (getsockopt(sockets[i], SOL_SOCKET, SO_ERROR,
				    &optval, &optlen) == -1) {
					err(1, "getsockopt");
				}

				/*
				 * getsockopt() puts the errno value for connect
				 * into optval so 0 means no-error.
				 */
				if (optval == 0)
					printf("  %s connected OK on %d\n",
					    hostnames[i], ports[i]);
				else {
					printf("  %s connect failed on %d (%s)\n",
					    hostnames[i], ports[i],
					    strerror(optval));
				}

				/* we no longer care about this socket */
				FD_CLR(sockets[i], &wrfds_orig);
				close(sockets[i]);
				++n;
			}
		}

		/* the shrinking set of sockets we still care about */
		wrfds = wrfds_orig;
	}

	/* those remaining in the wrfds set are those that timed out on us */
	for (i = 0; i < MAX_HOSTS; ++i) {
		if (FD_ISSET(sockets[i], &wrfds_orig))
			printf("  %s timed out on %d\n", hostnames[i], ports[i]);
	}

	return (0);
}
