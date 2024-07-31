#include "incs.h"
#include "copy_array.h"

int 
copy_array(
    void *x,
    size_t sz,
    void **ptr_y
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); }
  if ( sz <= 0 ) { go_BYE(-1); }
  void *y = malloc(sz);
  return_if_malloc_failed(y);
  memcpy(y, x, sz);
  *ptr_y = y;
BYE:
  return status;
}
