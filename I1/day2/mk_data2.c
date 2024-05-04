#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  char arr[6] = {228, 186, 186, 229, 191, 151};

  int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1)
  {
    perror("open");
    exit(1);
  }

  if (write(fd, arr, sizeof(arr)) != sizeof(arr))
  {
    perror("write");
    exit(1);
  }

  close(fd);

  return 0;
}
