#include <stdio.h>
int
main(
    )
{
  float x[] = { 1, 2, 3, 4, 5 };
  float y[] = { 6, 7, 8, 9, 10 };
  float z[5];
  vvdiv_F4(x, y, 5, z);
  for ( int i = 0; i < 5; i++ ) { 
    printf("%f/%f = %f \n", x[i], y[i], z[i]);
  }
}
