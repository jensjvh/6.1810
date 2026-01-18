#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char currentStr[512] = {0};
char buf[1];

void sixfive(int fd) {
  int n;

  while ((n = read(fd, buf, 1)) > 0) {
    char c = buf[0];

    if (c >= '0' && c <= '9') {
      int len = strlen(currentStr);
      if (len < sizeof(currentStr) - 1) {
        currentStr[len] = c;
        currentStr[len + 1] = '\0';
      }
      continue;
    }

    if (currentStr[0] != '\0') {
      int val = atoi(currentStr);
      if (val % 5 == 0 || val % 6 == 0) {
        int i = 0;
        while (currentStr[i] == '0') i++;
          char *numToPrint = (currentStr[i] == '\0') ? "0" : currentStr + i;

          write(1, numToPrint, strlen(numToPrint));
          write(1, "\n", 1);
      }
      currentStr[0] = '\0';
    }
  }


  if (currentStr[0] != '\0') {
    int val = atoi(currentStr);
    if (val % 5 == 0 || val % 6 == 0) {
        int i = 0;
        while (currentStr[i] == '0') i++;
          char *numToPrint = (currentStr[i] == '\0') ? "0" : currentStr + i;

          write(1, numToPrint, strlen(numToPrint));
          write(1, "\n", 1);
    }
  }

  if (n < 0) {
    fprintf(2, "sixfive: read error\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int fd;

  if (argc < 2) {
    fprintf(2, "sixfive: there should be at least 1 argument\n");
    exit(1);
  }

  for (int i = 1; i < argc; ++i) {
    if ((fd = open(argv[i], O_RDONLY)) < 0) {
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }

    sixfive(fd);
    close(fd);
  }

  exit(0);
}
