#include "qdf_struct.h"
#ifndef QDF_CHECKERS_H
#define QDF_CHECKERS_H
extern int
chk_qtype(
    qtype_t qtype
    );
extern int 
chk_mixed_array(
    char *x
    );
extern int
chk_qdf(
    const QDF_REC_TYPE * const ptr_qdf
    );
extern int
chk_hdr(
    void *x
    );
extern int
chk_jtype(
    jtype_t jtype,
    qtype_t qtype
    );
extern int 
chk_nil(
    void *x
    );
extern int 
chk_bool(
    void *x
    );
extern int 
chk_number(
    void *x
    );
extern int 
chk_date(
    void *x
    );
extern int 
chk_string(
    void *x
    );
extern int
chk_uniform_array(
    void *x,
    qtype_t qtype
    );
extern int 
chk_object(
    void *x
    );
extern int
chk_is_data_frame(
      const QDF_REC_TYPE *const ptr_qdf,
      bool *ptr_is_df
    );
extern int
are_keys_in_object(
      const QDF_REC_TYPE * const ptr_qdf,
      const char ** const keys, // [n_keys]
      uint32_t n_keys,
      bool *ptr_are_keys
    );
#endif // QDF_CHECKERS_H
