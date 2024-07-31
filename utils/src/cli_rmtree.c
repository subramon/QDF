#include <stdio.h>
#include <stdlib.h>
#include "utils_macros.h"
#include "rmtree.h"
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  if ( argc != 2 ) { go_BYE(-1); }
  status = rmtree(argv[1]); cBYE(status);
BYE:
  return status;
}
