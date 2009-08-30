/*
 * Start and then connect to 2222 from another terminal or another machine. You
 * will see that the system initializes the TCP connections even when the
 * program is not in the accept() call, and then accepts some data on it until
 * it reaches the buffer size. After that, it stops accepting more data, waiting
 * for some program to accept() and start reading.
 *
 * (c) jp@devnull.cz
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>

int main(int argc, char **argv)
{
	struct sockaddr_in in;
	int fd, newsock, n, optval = 1;

	bzero(&in, sizeof (in));
	in.sin_family = AF_INET;
	in.sin_port = htons(2222);
	in.sin_addr.s_addr = htons(INADDR_ANY);

	fd = socket(AF_INET, SOCK_STREAM, 0);

	/*
	 * So that we can reuse the port immediately. See the lecture materials
	 * for more info.
	 */
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	
	bind(fd, (struct sockaddr *)&in, sizeof(in));
	listen(fd, SOMAXCONN);

	/* wait for a signal */
	pause();

	return (0);
}
