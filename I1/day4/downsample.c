#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int ratio = atoi(argv[1]);
  int cnt = 0;
  while (1)
  {
    signed int buf[1];
    int n = read(0, buf, sizeof(buf[0]));
    if (n == -1)
    {
      perror("read");
      exit(1);
    }
    if (n == 0)
    {
      break;
    }
    if (cnt == 0)
    {
      write(1, buf, sizeof(buf[0]));
    }
    cnt = (cnt + 1) % ratio;
  }
}