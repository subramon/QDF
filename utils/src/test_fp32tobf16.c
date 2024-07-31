#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "fp32tobf16.h"
#include "bf16tofp32.h"

/*
static void print_hex(float x)
{
    int *p = (int*) &x;
    printf("%f=%x\n", x, *p);
}
*/

int 
main(
    void
    )
{
  float a[] = {1.2, 2.31, 3.46, 5.63, -1.2, -2.31, -3.46, -5.63 };
  float b[8];

  for ( int i = 0; i < 8; i++ ) {
    printf("FLOAT = %f \n", a[i]);
    // print_hex(a[i]); 
    uint32_t bf_a = fp32tobf16(a[i]);
    // Converting from bf16 to fp32
    b[i] = bf16tofp32(bf_a);
    printf("FLOAT %f \n\n", b[i]);
  }
  return 0;
}
