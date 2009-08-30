#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void)
{
	int fd;
	char c = (char) 'a';
	char *addr = NULL;

	fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666);

	lseek(fd, 100, SEEK_SET);
	write(fd, &c, 1);

	addr = mmap(0, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	//addr[4095] = 0;
	addr[4096] = 0;

	return 0;
}
