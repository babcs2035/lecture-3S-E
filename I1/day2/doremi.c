#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const double r1 = 1.0594630943592952;
const double r2 = r1 * r1;
const double f_ = 440 * r2 * r1 / 2;

int main(int argc, char **argv)
{
  int A = atoi(argv[1]);
  int n = atoi(argv[2]);

  short arr[n * 13230];

  double f = f_;
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < 13230; ++j)
    {
      arr[13230 * i + j] = A * sin(2. * M_PI * f * j / 44100);
    }
    if (i == 2 || i == 5)
      f *= r2;
    else
      f *= r1;
  }

  write(1, arr, sizeof(arr));

  return 0;
}