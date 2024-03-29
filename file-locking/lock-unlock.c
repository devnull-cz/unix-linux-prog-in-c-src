/*
 * A simple program to provide file locking.  It has 2 parameters, 1st is the
 * lock filename, 2nd one specifies whether you lock or unlock.  It can NOT
 * recover from the situation when somebody leaves the file lock behind (and
 * exits, for example).  I.e. it does not know how to figure out whether the
 * lock is "stale".
 *
 * It might be tempting to tackle the problem of stale locks by writing the PID
 * of the process owning the lock to the file, then check whether the process is
 * still alive. If not, you write new PID to the lock file.  However, the
 * operation of reading the PID, checking its existence, and writing the new
 * lock file is again NOT atomic so this can lead to incorrect situations.
 *
 * See run.sh in this directory as an example script that uses this program.
 *
 * The app that uses this simple locking mechanism must busy wait.  That is
 * usually not acceptable in the real world.
 *
 * If successful, the program returns 0 if the lock/unlock operation succeeded,
 * 1 if it cannot lock since it is already locked, and 2 if either of those
 * operations failed unexpectedly.
 *
 * See ./run.sh for more information.
 *
 * (c) jp@devnull.cz
 */

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define	OPL	"lock"
#define	OPUNL	"unlock"

int
lock(char *filename)
{
	if (open(filename, O_RDONLY | O_TRUNC | O_EXCL | O_CREAT, 0666) != -1)
		return (0);

	if (errno != EEXIST)
		err(2, "open");
	return (1);
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
		errx(1, "usage: %s <lockfile> %s | %s", argv[0], OPL, OPUNL);

	if (strcmp(argv[2], OPL) != 0 && strcmp(argv[2], OPUNL) != 0)
		errx(1, "usage: %s <lockfile> %s | %s", argv[0], OPL, OPUNL);

	if (strcmp(argv[2], OPL) == 0)
		return (lock(argv[1]));

	return (unlock(argv[1]));
}
