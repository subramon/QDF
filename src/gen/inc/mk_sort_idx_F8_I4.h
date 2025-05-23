#include "incs.h"
#include "macros.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"

typedef struct _sort_struct_F8_I4_t {
  uint32_t idx; // idx is always I4
  double val1_F8;
  int32_t val2_I4;
} sort_struct_F8_I4_t;

extern int
mk_sort_idx_F8_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
 
