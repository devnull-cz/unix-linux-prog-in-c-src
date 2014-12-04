/*
 * Example on how to work with a non-blocking connect. Uses fixed input and
 * should show all 3 situations we care about - a successful connect, a refused
 * connect, and a timeout.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
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

#define	TIMEOUT		10
#define	MAX_SOCKETS	3

typedef struct host_s {
	char *hostname;
	char *port;
	int sockets[MAX_SOCKETS];
	int numsock;
} host_t;

host_t hosts[] = {
	/* This should connect OK. */
	{ "www.devnull.cz", "80", {-1, -1, -1}, 0 },
	/* This will timeout. */
	{ "www.devnull.cz", "33", {-1, -1, -1}, 0 },
	/* This should refuse the connection on both IPv4 and IPv6. */
	{ "mail.kolej.mff.cuni.cz", "999", {-1, -1, -1}, 0 },
	/* To see if localhost gets EINPROGRESS as well. */
	{ "localhost", "22", {-1, -1, -1}, 0 },
	/*
	 * To see if localhost gets EINPROGRESS as well. Connection refusal
	 * might be different from an open port. Port 7 is echo service,
	 * nobody should run it these days.
	 */
	{ "localhost", "7", {-1, -1, -1}, 0 },
	{ NULL, 0, {-1, -1, -1}, 0 }
};

int
main(int argc, char **argv)
{
	socklen_t optlen;
	struct hostent *he;
	struct timeval tout;
	fd_set wrfds, wrfds_orig;
	int flags, i, optval, n, timeouts, error, num_sockets = 0;
	struct addrinfo *res, *resorig, hints;
	host_t *hostp;
	char ip_str[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	printf("will work with these machines/services:\n");
	for (hostp = hosts; hostp->hostname != NULL; hostp++)
		printf("  %s:%s\n", hostp->hostname, hostp->port);

	printf("making all sockets non-blocking, and calling connect...\n");
	FD_ZERO(&wrfds);
	n = 0; /* counts finished (succeeded/failed) connects */

	/* In this cycle we initiate connects to all the hosts. */
	for (hostp = hosts; hostp->hostname != NULL; hostp++) {
		if ((error = getaddrinfo(hostp->hostname, hostp->port,
		    &hints, &res)) != 0)
			errx(1, "%s", gai_strerror(error));

		i = 0;
		/* One hostname can map to multiple addresses. */
		for (resorig = res; res != NULL; res = res->ai_next, i++) {
			if (res->ai_family != AF_INET &&
			    res->ai_family != AF_INET6)
				continue;

			/* Convert IP address to string. */
			if ((error = getnameinfo(res->ai_addr, res->ai_addrlen,
			    ip_str, sizeof (ip_str), NULL, 0,
			    NI_NUMERICHOST) != 0)) {
				errx(1, "%s", gai_strerror(error));
			}

			if ((hostp->sockets[i] = socket(res->ai_family,
			    res->ai_socktype, 0)) == -1) {
				warn("socket() failed for %s", ip_str);
				continue;
			}
			hostp->numsock++;

			printf("  %s[%s]:%s socket %d\n", hostp->hostname,
			    ip_str, hostp->port, hostp->sockets[i]);

			flags = fcntl(hostp->sockets[i], F_GETFL);
			if (fcntl(hostp->sockets[i], F_SETFL,
			    flags | O_NONBLOCK) == -1)
				err(1, "fcntl");

			if (connect(hostp->sockets[i],
			    (struct sockaddr *)res->ai_addr,
			    res->ai_addrlen) == -1) {
				/* This is what we expect. */
				if (errno == EINPROGRESS) {
					printf("    connect EINPROGRESS OK "
					    "(expected)\n");
					FD_SET(hostp->sockets[i], &wrfds);
				} else {
					/*
					 * This may happen right here, on
					 * localhost for example (immediate
					 * connection refused).
					 * I can see that happen on FreeBSD
					 * but not on Solaris, for example.
					 */
					printf("    connect: %s\n",
					    strerror(errno));
					++n;
				}
			} else {
				/* This may happen, on localhost for example */
				printf("  %s connected OK on port %s\n",
				    hostp->hostname, hostp->port);
				++n;
			}
			num_sockets++;
		}
		freeaddrinfo(resorig);
	}

	/* Save our original fd set. */
	wrfds_orig = wrfds;
	/* Let's print some progress every second. */
	tout.tv_sec = 1;
	tout.tv_usec = 0;

	printf("continuing with select now, checking the connect status...\n");
	timeouts = 0;
	while (n != num_sockets) {
		/*
		 * Note that non-blocking connect uses the WR set, not the RD
		 * one.
		 */
		error = select(FD_SETSIZE, NULL, &wrfds, NULL, &tout);

		if (error == 0) {
			if (++timeouts == TIMEOUT) {
				printf("  TIMEOUT REACHED\n");
				break;
			}
			printf("  .\n");
			/*
			 * Select may change the timeval structure (see the
			 * spec) so reset it to original values.
			 */
			tout.tv_sec = 1;
			tout.tv_usec = 0;
			continue;
		} else {
			if (error == -1)
				err(1, "select");
		}

		/* Check all hosts. */
		for (hostp = hosts; hostp->hostname != NULL; hostp++) {
			/* Check all sockets for this host. */
			for (i = 0; i < hostp->numsock; i++) {
				if (!FD_ISSET(hostp->sockets[i], &wrfds))
					continue;

				optval = -1;
				optlen = sizeof (optval);

				if (getsockopt(hostp->sockets[i],
				    SOL_SOCKET, SO_ERROR, &optval,
				    &optlen) == -1)
					err(1, "getsockopt");

				/*
				 * getsockopt() puts the errno value
				 * for connect into optval so 0 means
				 * no-error.
				 */
				if (optval == 0) {
					printf("  %s connected OK on "
					    "port %s socket %d\n",
					    hostp->hostname, hostp->port,
					    hostp->sockets[i]);
				} else {
					printf("  %s connect failed on "
					    "port %s socket %d (%s)\n",
					    hostp->hostname, hostp->port,
					    hostp->sockets[i],
					    strerror(optval));
				}

				/* no longer care about this socket */
				FD_CLR(hostp->sockets[i], &wrfds_orig);
				close(hostp->sockets[i]);
				++n;
			}
		}

		/* the shrinking set of sockets we still care about */
		wrfds = wrfds_orig;
	}

	/* those remaining in the wrfds set are those that timed out on us */
	for (hostp = hosts; hostp->hostname != NULL; hostp++) {
		for (i = 0; i < hostp->numsock; i++) {
			if (FD_ISSET(hostp->sockets[i], &wrfds_orig))
				printf("  %s timed out on port %s socket %d\n",
				    hostp->hostname, hostp->port,
				    hostp->sockets[i]);
		}
	}

	return (0);
}
