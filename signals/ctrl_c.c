#define	_XOPEN_SOURCE	700

#include <string.h>
#include <signal.h>
#include <unistd.h>

#define	MESSAGE "Signal caught !\n"

void
ctrl_c(int sig)
{
	write(1, MESSAGE, strlen(MESSAGE));
}

int
main(void)
{
	struct sigaction act = { 0 };

	act.sa_handler = ctrl_c;
	sigaction(SIGINT, &act, NULL);

	while (1) {
		sleep(1);
	}

	return (0);
}
