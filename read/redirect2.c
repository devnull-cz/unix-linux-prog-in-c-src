/*
 * Implement the following command:
 *
 *	cat $1 >out 2>&1
 *
 * Run like this:
 *
 *	gcc redirect2.c
 *	rm -f out
 *	./a.out /etc/passwd
 *	diff /etc/passwd out
 *	rm -f out
 *	./a.out /nonexistent
 *	cat out
 *
 * Also run like this, the output ends up closed and the "out" file will stay
 * empty after it is created.
 *
 *	rm -f out
 *	./a.out <&- /etc/passwd
 *	wc -l out
 *		0 out
 */
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

int
main(int argc, char **argv)
{
	assert(argc == 2);

	/* Do the ">out" part. */
	close(1);
	if (creat("out", 0666) == -1)
		err(1, "creat");

	/* Do the 2>&1 part. */
	close(2);
	dup(1);

	execl("/bin/cat", "cat", argv[1], NULL);
	err(1, "execl");

	/* Not reached. */
	return (1);
}
