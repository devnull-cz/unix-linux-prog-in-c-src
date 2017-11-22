/* example: rochkind - advanced unix programming */

#include <sys/un.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define	SOCKETNAME "my_socket"

int
main(void)
{
	struct sockaddr_un sa;
	int fd_skt, fd_client;
	char buf[100]; /* I/O buffer */

	unlink(SOCKETNAME);
	/* note: actual size of sun_path array differs accross systems */
	strncpy(sa.sun_path, SOCKETNAME, sizeof (sa.sun_path));
	sa.sun_family = AF_UNIX;

	if (fork() == 0) {
		/*
		 * also try with protocol number 6 or 17; why socket returns
		 * an error then?
		 */
		if ((fd_skt = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
			perror("socket");
			exit(1);
		}
		sleep(1);

		connect(fd_skt, (struct sockaddr *) &sa, sizeof (sa));

		write(fd_skt, "hello!", 7);
		read(fd_skt, buf, sizeof (buf));
		printf("client got \"%s\"\n", buf);
	} else {
		if ((fd_skt = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
			perror("socket");
			exit(1);
		}
		bind(fd_skt, (struct sockaddr *) &sa, sizeof (sa));
		listen(fd_skt, SOMAXCONN);
		fd_client = accept(fd_skt, NULL, 0);

		read(fd_client, buf, sizeof (buf));
		printf("server got \"%s\"\n", buf);
		write(fd_client, "get out!", 9);
	}

	return (0);
}
