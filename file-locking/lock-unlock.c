/*
 * Simple program to provide file locking. It has 2 parameters, 1st is the lock
 * filename, 2nd one denotes whether you lock or unlock. It can NOT recover from
 * the situation when somebody leaves the file lock there (and exists, for
 * example).
 *
 * The app that uses this simple locking mechanism must busy wait. That's
 * usually not acceptable in the real life. See run.sh in this directory.
 *
 * If successful, the program returns 0 if locked or unlocked, 1 if it can't
 * lock since it's already locked, and 2 if either of those operations failed
 * unexpectedly.
 *
 * See ./run.sh for more information.
 *
 * (c) jp@devnull.cz
 */

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <string.h>

int
lock(char *filename)
{
	if (open(filename, O_RDONLY | O_TRUNC | O_EXCL | O_CREAT, 0666) == -1) {
		if (errno != EEXIST)
			err(2, "open");
		return (1);
	}
	return (0);		
}

int
unlock(char *filename)
{
	if (unlink(filename) == -1)
		err(2, "unlink");
	return (0);		
}

int
main(int argc, char **argv)
{
	if (argc != 3)
		errx(1, "usage: %s <lockfile> lock | unlock", argv[0]);

	if (strcmp(argv[2], "lock") != 0 && strcmp(argv[2], "unlock") != 0)
		errx(1, "usage: %s <lockfile> lock | unlock", argv[0]);

	if (strcmp(argv[2], "lock") == 0)
		return (lock(argv[1]));
	else
		return (unlock(argv[1]));
}
