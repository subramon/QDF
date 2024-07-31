#include "incs.h"
#include "macros.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"

typedef struct _sort_struct_F8_F8_t {
  uint32_t idx; // idx is always I4
  double val1_F8;
  double val2_F8;
} sort_struct_F8_F8_t;

extern int
mk_sort_idx_F8_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
 
