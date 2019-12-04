/*
 * See how AI_PASSIVE flag is relevant.
 * Try with/without hostname and with/without the flag.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700
#define	_GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <libgen.h>
#include <stdbool.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int error;
	struct addrinfo *res, *resorig, hints;
	bool passive = false;
	char *hostname = NULL;

	if (argc < 2) {
		errx(1, "usage: %s [-P] <service> [<hostname>]",
		    basename(argv[0]));
	}

	if (strncmp(argv[1], "-P", 2) == 0) {
		printf("passive mode\n");
		passive = true;
		argv++;
		argc--;
	}

	if (argv[2] != NULL) {
		hostname = argv[2];
		printf("hostname = %s\n", hostname);
	}

	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (passive)
		hints.ai_flags = AI_PASSIVE;

	if ((error = getaddrinfo(hostname, argv[1], &hints, &res)) != 0)
		errx(1, "%s", gai_strerror(error));

	for (resorig = res; res != NULL; res = res->ai_next) {
		int s = socket(res->ai_family, res->ai_socktype,
		    res->ai_protocol);
		if (s == -1) {
			warn("socket");
			continue;
		}

		if (bind(s, res->ai_addr, res->ai_addrlen) == -1) {
			warn("bind");
			continue;
		}

		close(s);
	}

	freeaddrinfo(resorig);

	return (0);
}
