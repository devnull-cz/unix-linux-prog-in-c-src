#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	char buf[100];
	int fd, n;
	struct sockaddr_in sa;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(2222);
	inet_aton("127.0.0.1", &sa.sin_addr);

	fd = socket(AF_INET, SOCK_STREAM, 0);

	do {
		if ((n = read(0, buf, 100)) <= 0) {
			close(fd);
			fprintf(stderr, "read closed\n");
			n = 1;
			break;
		}

	} while ((n = sendto(fd, buf, n, 0, (struct sockaddr *) &sa,
	    sizeof (sa))) > 0);

	if (n != 1) {
		fprintf(stderr, "write closed\n");
		close(fd);
	}

	return (0);
}
