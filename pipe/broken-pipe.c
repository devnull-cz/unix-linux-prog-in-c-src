#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  int pd[2];
  char c = (char) 'a';

  pipe(pd);
  close(pd[0]);
  write(pd[1], &c, 1);

  return 0;
}
