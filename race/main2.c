#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/stat.h>

int i, j;

void print_stat(int sig)
{
  printf("\nstats: inconsistency %d of %d\n", j, i);
  _exit(0);
}

int main(void)
{
  int fd;
  char *addr = NULL;
  char c = 0, a, b;
  struct sigaction act;
  key_t key;
  int sem;
  struct sembuf down = { 0, -1, 0};
  struct sembuf up = { 0, 1, 0};

  key = ftok("/etc/passwd", 0);
  if ((sem = semget(key, 1, IPC_CREAT | S_IRUSR | S_IWUSR)) == -1) {
    perror("semget");
    exit(1);
  }

  if (semctl(sem, 0, SETVAL, 1) == -1) {
    fprintf(stderr, "%d: %s\n", errno, strerror(errno));
    exit(1);
  }

  bzero(&act, sizeof(act));
  act.sa_handler = print_stat;
  sigaction(SIGINT, &act, NULL);

  if ((fd = open("test.dat", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
    perror("open");
    exit(1);
  }

  write(fd, &c, 1);
  write(fd, &c, 1);

  addr = mmap(0, 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if ((void *) addr == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  switch (fork()) {
    case -1: perror("fork"); exit(1);
    case  0: 
      while (1) {
	if (semop(sem, &down, 1) == -1) {
	  perror("semop");
	  exit(1);
	}
	a = addr[0];
	b = addr[1];
	if (semop(sem, &up, 1) == -1) {
	  perror("semop");
	  exit(1);
	}

	if (a != b) {
	  fprintf(stderr, "[child  (%d/%d)] ", a, b);
	  ++j;
	}

	if (semop(sem, &down, 1) == -1) {
	  perror("semop");
	  exit(1);
	}
	addr[0] = 2;
	addr[1] = 2;
	if (semop(sem, &up, 1) == -1) {
	  perror("semop");
	  exit(1);
	}
	++i;
      }
      break;
    default:
      while (1) {
	if (semop(sem, &down, 1) == -1) {
	  perror("semop");
	  exit(1);
	}
	a = addr[0];
	b = addr[1];
	if (semop(sem, &up, 1) == -1) {
	  perror("semop");
	  exit(1);
	}

	if (a != b) {
	  fprintf(stderr, "[PARENT (%d/%d)] ", a, b);
	  ++j;
	}

	if (semop(sem, &down, 1) == -1) {
	  perror("semop");
	  exit(1);
	}
	addr[0] = 1;
	addr[1] = 1;
	if (semop(sem, &up, 1) == -1) {
	  perror("semop");
	  exit(1);
	}
	++i;
      }
      break;
  }

  munmap(addr, 2);
  close(fd);
  return 0;
}
