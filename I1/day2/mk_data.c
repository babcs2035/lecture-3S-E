#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int8_t arr[6] = {228, 186, 186, 229, 191, 151};

  int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  write(fd, arr, sizeof(arr));
  close(fd);

  return 0;
}