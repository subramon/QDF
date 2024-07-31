#include "qdf_struct.h"
#ifndef QDF_HELPERS_H
#define QDF_HELPERS_H
extern int
set_is_df(
    void *x,
    bool  is_df
    );
extern bool
get_is_df(
    const void * const x
    );
extern jtype_t 
get_jtype(
    const void * const x
    );
extern qtype_t 
get_qtype(
    const void * const x
    );
extern uint32_t
get_poff(
    const void * const x
    );
extern uint32_t
get_str_len(
    const void * const x
    );
extern uint32_t
get_str_size(
    const void * const x
    );
extern uint32_t
get_arr_len(
    const void * const x
    );
extern uint32_t
get_arr_size(
    const void * const x
    );
extern uint32_t
get_obj_len(
    const void * const x
    );
extern double
get_num_val(
    const void * const x
    );
extern int
get_date_val(
    const void * const x,
    tm_t *ptr_tm
    );
extern char * 
get_str_val(
    void * x
    );
extern bool
get_bool_val(
    const void * const x
    );
extern int
free_qdf(
    QDF_REC_TYPE *ptr_qdf
    );
extern int
copy_qdf(
    QDF_REC_TYPE *ptr_dst,
    const QDF_REC_TYPE * const ptr_src
    );
extern int
get_arr_val(
    void * const x,
    uint32_t idx,
    SCLR_REC_TYPE *out_sclr,
    QDF_REC_TYPE *out_qdf
    );
extern int
get_keys(
    const QDF_REC_TYPE * const ptr_qdf_df,
    QDF_REC_TYPE *ptr_qdf_keys
    );
extern int
is_val_in_SC_array(
    const QDF_REC_TYPE * const ptr_qdf, // input 
    const char * const key, // input 
    int32_t *ptr_idx  // output 
    );
extern int
get_key_val(
    const QDF_REC_TYPE *const ptr_in,
    int nidx, // key described as index 
    const char * const sidx, // key described as string
    QDF_REC_TYPE *ptr_out,
    uint32_t *ptr_offset // how far the output col is from start of dataframe
   );
extern int
get_arr_idx(
    void * x, // input 
    SCLR_REC_TYPE *out, // input 
    int32_t *ptr_idx // output 
    );
extern int
is_key(
    const QDF_REC_TYPE *const ptr_in,
    const char * const sidx, // key described as string
    bool *ptr_is_key
   );
extern bool
is_nil(
    const QDF_REC_TYPE *const ptr_in
    );
extern void
zero_qdf(
    QDF_REC_TYPE *ptr_qdf
    );
extern uint32_t * 
get_offsets_in_arr(
    void * x
    );
extern void *
get_offsets_in_obj(
    void * x
    );
extern void *
get_keys_in_obj(
    void * x
    );
extern uint32_t
get_qdf_size(
    const void * x
    );
extern void
set_jtype(
    void *x,
    jtype_t jtype
    );
extern void
set_qtype(
    void *x,
    qtype_t  qtype
    );
extern int
set_str_len(
    void *x,
    uint32_t str_len
    );
extern int
set_str_size(
    void *x,
    uint32_t str_size
    );
extern int
set_num_val(
    void *x,
    double dval
    );
extern int
set_date_val(
    void *x,
    tm_t date_val
    );
extern int
set_str_val(
    void *x,
    const char * const str_val 
    );
extern int
set_bool_val(
    void *x,
    bool bval
    );
extern int
set_qdf_size(
    void *x,
    uint32_t qdf_size
    );
extern int
set_arr_width(
    void *x,
    uint32_t  width
    );
extern uint32_t
get_arr_width(
    const void * const x
    );
extern int
set_arr_len(
    void *x,
    uint32_t arr_len
    );
extern int
set_obj_len(
    void *x,
    uint32_t arr_len
    );
extern int
set_arr_size(
    void *x,
    uint32_t arr_size
    );
extern int
set_obj_arr_size(
    void *x,
    uint32_t arr_size
    );
extern uint32_t
get_obj_arr_size(
    const void * const x
    );
extern int
set_obj_arr_len(
    void *x,
    uint32_t arr_len
    );
extern uint32_t
get_obj_arr_len(
    const void * const x
    );
extern int 
get_sclr_as_F8(
    const SCLR_REC_TYPE *const ptr_sclr,
    double *ptr_x
    );
extern void * 
get_arr_ptr(
    void * x
    );
extern int 
get_keys_as_array(
    QDF_REC_TYPE *ptr_qdf,
    char ***ptr_keys,
    uint32_t *ptr_n_keys
    );
extern int 
get_str_qtypes_as_array(
    QDF_REC_TYPE *ptr_qdf,
    char ***ptr_str_qtypes, 
    uint32_t *ptr_n_str_qtypes
    );
#endif //  QDF_HELPERS_H
