#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  int8_t arr[256];
  for (int i = 0; i < 256; ++i)
  {
    arr[i] = i;
  }

  int fd = open("my_data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  write(fd, arr, sizeof(arr));
  close(fd);

  return 0;
}
