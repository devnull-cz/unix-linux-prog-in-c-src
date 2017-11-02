/*
 * Trivial reflection TCP service supposed to run from inetd with
 * 'wait' wait-status therefore it must accept() on fd 0 passed from inetd
 * to get a socket on which it can do I/O.
 *
 * inetd.conf line for the service should look like this:
 *
 *   myserv stream tcp wait root /usr/local/bin/foo foo
 *
 * Assuming the program was compiled into /usr/local/bin/foo and
 * /etc/services file has entry called 'myserv' for a TCP port.
 *
 * (c) vlada@devnull.cz
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <err.h>

int
main(void)
{
	int client_fd;
	char buf[BUFSIZ];
	int len;

	client_fd = accept(STDIN_FILENO, NULL, 0);
	if (client_fd == -1)
		err(1, "accept failed");

	while ((len = read(client_fd, buf, sizeof (buf))) > 0) {
		if (write(client_fd, buf, len) < 0)
			err(1, "write failed");
	}

	return (0);
}
