/*
 * Demonstrate reentrant (or lack of) of inet_ntoa()
 *
 * See INET(3) man page on FreeBSD for inet_ntoa_r()
 *
 * Vladimir Kotal, 2010
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main(void)
{
	in_addr_t ip1 = inet_addr("1.2.3.4");
	in_addr_t ip2 = inet_addr("5.6.7.8");
	struct in_addr addr1, addr2;
#ifdef __FreeBSD__
	char buf[24], buf2[24];
#endif

	addr1.s_addr = ip1;
	addr2.s_addr = ip2;

	printf("one: %s\n", inet_ntoa(addr1));
	printf("two: %s\n", inet_ntoa(addr2));
	printf("one+two: %s %s\n",
	    inet_ntoa(addr1), inet_ntoa(addr2));
#ifdef __FreeBSD__
	printf("one+two: %s %s\n",
	    inet_ntoa_r(addr1, buf, sizeof (buf)),
	    inet_ntoa_r(addr2, buf2, sizeof (buf2)));
#endif
}
