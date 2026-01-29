#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  int i, j, x = 0;
  for(i = 0; i < 1000; i++)
    for(j = 0; j < 1000000; j++)
      x += j;
  printf(1, "pid %d done: %d\n", getpid(), x);
  exit();
}
