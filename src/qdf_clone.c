#include "incs.h"
#include "qdf_struct.h"
#include "qdf_clone.h"

int
qdf_clone(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE * dst
   )
{
  int status = 0;

  mcr_chk_non_null(src, -1);
  mcr_chk_null(dst, -1);
  dst->size = src->size;
  status = posix_memalign((void **)&(dst->data), 16, src->size);
  cBYE(status);
  memcpy(dst->data, src->data, src->size);
BYE:
  return status;
}
