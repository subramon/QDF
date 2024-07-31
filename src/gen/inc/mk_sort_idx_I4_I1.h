#include "incs.h"
#include "macros.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"

typedef struct _sort_struct_I4_I1_t {
  uint32_t idx; // idx is always I4
  int32_t val1_I4;
  int8_t val2_I1;
} sort_struct_I4_I1_t;

extern int
mk_sort_idx_I4_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
 
