#include "qdf_struct.h"
extern QDF_REC_TYPE *
qdf_cmem_make(
    uint32_t sz,
    const char * const name // for debugging 
    );
extern QDF_REC_TYPE *
qdf_cmem_clone(
    QDF_REC_TYPE *x
    );
extern QDF_REC_TYPE *
qdf_cmem_dupe(
    QDF_REC_TYPE *y
    );
extern int 
qdf_cmem_free(
  QDF_REC_TYPE *x
);
extern void
qdf_cmem_nop(
  QDF_REC_TYPE *x
);
extern int 
qdf_cmem_set_name(
  QDF_REC_TYPE *x,
  const char * const name
);
extern char * 
qdf_cmem_get_name(
  QDF_REC_TYPE *x
);
