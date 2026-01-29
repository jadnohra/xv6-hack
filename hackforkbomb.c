#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  fork();
  fork();
  exec("hackspin", (char*[]){ "hackspin", 0 });
  exit();
}
