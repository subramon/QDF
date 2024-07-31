#include "qdf_struct.h"
extern int
shift_up_down(
    const QDF_REC_TYPE * const src, // input numeric array 
    uint32_t shift_by,
    const char * const direction,
    QDF_REC_TYPE * restrict dst, // output numeric array (same type as src)
    QDF_REC_TYPE * restrict nn_dst // output numeric array I1
   );
