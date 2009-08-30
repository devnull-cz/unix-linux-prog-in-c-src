#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>

int main(void)
{
  printf("real ID:\t%s\n", getpwuid(getuid())->pw_name);
  printf("effective ID:\t%s\n", getpwuid(geteuid())->pw_name);
  printf("real GID:\t%s\n", getgrgid(getgid())->gr_name);
  printf("effective GID:\t%s\n\n", getgrgid(getegid())->gr_name);

  if (open("test", O_RDONLY) == -1) {
    perror("test");
  }
  else {
    printf("ok, open() succeeded\n");
  }

  printf("\nafter setuid()...\n\n");

  /* if (setreuid(162948, 162948) == -1) {*/
/*  if (setreuid(113369, 113369) == -1) {
    perror("setreuid");
    exit(1);
  }
  */

  /* if (setreuid(162948, 162948) == -1) {*/
  if (setuid(162948) == -1) {
    perror("setuid");
    exit(1);
  }

  /* if (setreuid(162948, 162948) == -1) {*/
  if (setuid(113369) == -1) {
    perror("setuid2");
    exit(1);
  }

  printf("real ID:\t%s\n", getpwuid(getuid())->pw_name);
  printf("effective ID:\t%s\n", getpwuid(geteuid())->pw_name);
  printf("real GID:\t%s\n", getgrgid(getgid())->gr_name);
  printf("effective GID:\t%s\n\n", getgrgid(getegid())->gr_name);

  if (open("test", O_RDONLY) == -1) {
    perror("test");
  }
  else {
    printf("ok, open() succeeded\n");
  }

  return 0;
}
