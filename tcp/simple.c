#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char buf[100];
	int fd, newsock, n;
	struct sockaddr_in sa;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(2222);
	sa.sin_addr.s_addr = htons(INADDR_ANY);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	
	bind(fd, (struct sockaddr *) &sa, sizeof(sa));
	listen(fd, SOMAXCONN);

	for ( ; ; ) {
		newsock = accept(fd, NULL, 0);
		while ((n = read(newsock, buf, 100)) != 0)
			write(1, buf, n);
		close(newsock);
		fprintf(stderr, "-- connection closed --\n");
	}
	return 0;
}

/*
optval = 1;
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(optval));
*/
