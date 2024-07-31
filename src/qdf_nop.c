#include "incs.h"
#include "qdf_struct.h"
#include "qdf_nop.h"
int
nop3(
    SCLR_REC_TYPE *x
    )
{
  if ( x == NULL ) { WHEREAMI; return 1; } 
  printf("nop3\n"); return 0;
}

int
nop2(
    PRIMITIVE_VAL_TYPE *x
    )
{
  if ( x == NULL ) { WHEREAMI; return 1; } 
  printf("nop2\n"); return 0;
}

int
nop(
    QDF_REC_TYPE *x
    )
{
  if ( x == NULL ) { WHEREAMI; return 1; } 
  printf("nop\n"); return 0;
}

