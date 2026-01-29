#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int n = getnumsyscalls();
    printf(1, "syscalls so far: %d\n", n);
    exit();
}
