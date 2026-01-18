#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int duration;

  if (argc != 2) {
    fprintf(2, "sleep: 1 argument required\n");
    exit(1);
  }

  duration = atoi(argv[1]);
  
  pause(duration);
  exit(0);
}
