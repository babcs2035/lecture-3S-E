/*
 * p04.c
 */

/*
 * usage:
 *
 *   ./a.out Ax Ay Az Bx By Bz
 *
 * Intented behavior:
 * It should print the angle between vector A and B, where
 * A is (Ax, Ay, Az)
 * B is (Bx, By, Bz), using the formula you should have learned
 * in high schools.
 *                 A * B       (dot product)
 *  cos(theta) = ---------
 *                |A| |B|
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct vect3
{
  double x;
  double y;
  double z;
} vect3;

/* dot product (naiseki) */
double dot(vect3 *A, vect3 *B)
{
  return A->x * B->x + A->y * B->y + A->z * B->z;
}

double angle(vect3 *A, vect3 *B)
{
  return acos(dot(A, B) / sqrt(dot(A, A) * dot(B, B)));
}

void mk_point(vect3 *p, double x, double y, double z)
{
  p->x = x;
  p->y = y;
  p->z = z;
  return;
}

int main(int argc, char **argv)
{
  vect3 *A, *B;
  A = (vect3 *)malloc(sizeof(vect3));
  B = (vect3 *)malloc(sizeof(vect3));
  mk_point(A, atof(argv[1]), atof(argv[2]), atof(argv[3]));
  mk_point(B, atof(argv[4]), atof(argv[5]), atof(argv[6]));
  double a = angle(A, B);
  printf("%f\n", a);
  return 0;
}
