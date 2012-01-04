/*
 * Example on gethostbyname(). Note that this function is obsolete. You should
 * use getnameinfo() instead. Use with "www.google.com", for example, to see
 * aliases as well.
 *
 * Note that gethostbyname() and inet_ntoa() are obsolete and should not be used.
 * Use getaddrinfo() and inet_ntop() instead.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <libgen.h>

int
main(int argc, char **argv)
{
	int i;
	struct hostent *h;

	if (argc != 2)
		errx(1, "usage: %s <hostname>", basename(argv[0]));

	if ((h = gethostbyname(argv[1])) == NULL)
		hstrerror(h_errno);
	else {
		printf("official name:\n\t%s\n", h->h_name);

		/* go through aliases */
		printf("\naliases:\n");
		for (i = 0; h->h_aliases[i] != NULL; i++)
			printf("\t%s\n", h->h_aliases[i]);

		/* go through IP addresses */
		if (h->h_addrtype != AF_INET)
			return (0);
			
		/*
		 * h_addr_list contains address structures corresponding to
		 * the h_addrtype.
		 */
		printf("\nIP addresses:\n");
		for (i = 0; h->h_addr_list[i] != NULL; i++)
			/*
			 * Note that h->h_addr_list[x] is a pointer to a
			 * in_addr structure but inet_ntoa() wants in_addr
			 * structure itself.
			 */
			printf("\t%s\n",
			    inet_ntoa(*((struct in_addr *)h->h_addr_list[i])));
	}

	return (0);
}
