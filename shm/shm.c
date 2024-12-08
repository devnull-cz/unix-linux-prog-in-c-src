#define	_XOPEN_SOURCE	700	// for ftruncate on Linux

#include <sys/mman.h>
#include <sys/stat.h>	/* For mode constants */
#include <fcntl.h>	/* For O_* constants */

#include <unistd.h>
#include <sys/types.h>

#include <assert.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	if (argc != 3)
		errx(1, "usage: shm <shm_name> <char>");

	const char *shm_name = argv[1];
	if (shm_name[0] != '/')
		errx(1, "The name should have a leading slash.");

	int fd = shm_open(shm_name, O_RDWR | O_CREAT,
	    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd == -1)
		err(1, "shm_open");

	size_t size = 4444;
	assert(ftruncate(fd, size) == 0);

	// There would be no write-back with MAP_PRIVATE.
	char *p = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	assert(p);

	// This should not affect the memory mapping.
	close(fd);

	for (char *q = p; q < p + size; q++)
		*q = argv[2][0];

	// Uncomment to destroy the shared memory segment:
	// shm_unlink(shm_name);
}
