#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1)
  {
    perror("open");
    exit(1);
  }

  int index = 0;
  while (1)
  {
    short buf[1];
    int n = read(fd, buf, sizeof(buf[0]));
    if (n == -1)
    {
      perror("read");
      exit(1);
    }
    if (n == 0)
    {
      break;
    }
    printf("%d\t%d\n", index, buf[0]);
    ++index;
  }

  close(fd);

  return 0;
}
