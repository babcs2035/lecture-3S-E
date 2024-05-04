/*
 * p06.c
 */

/*
 * usage:
 *
 *   ./a.out
 *
 * Intented behavior:
 * It should print the content of this file.
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *fp = fopen("p06.c", "r");
  char buf[100];
  while (1)
  {
    int n = fread(buf, 1, 100, fp);
    if (n == 0)
      break;
    fwrite(buf, 1, n, stdout);
  }
  return 0;
}
