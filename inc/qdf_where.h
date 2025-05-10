#include "qdf_struct.h"
extern int
qdf_where(
    QDF_REC_TYPE *ptr_src, // INPUT df consisting of numeric arrays
    const QDF_REC_TYPE *const where, // numeric I1 array (1=> good, 0 => null)
    QDF_REC_TYPE *ptr_dst, // OUTPUT: same as INPUT 
    uint32_t *ptr_num_good
   );
