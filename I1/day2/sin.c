#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int A = atoi(argv[1]);
  int f = atoi(argv[2]);
  int n = atoi(argv[3]);

  short arr[n];
  for (int i = 0; i < n; ++i)
  {
    arr[i] = A * sin(2. * M_PI * f * i / 44100);
  }

  write(1, arr, sizeof(arr));

  return 0;
}
