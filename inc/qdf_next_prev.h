typedef enum { 
  cmp_undef,
  cmp_eq,
  cmp_neq,
  cmp_lt,
  cmp_leq,
  cmp_gt,
  cmp_geq
} cmp_t;

typedef enum { 
  next_prev_undef,
  is_next,
  is_prev,
} next_prev_t;

extern int
qdf_next_prev(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const str_op,
    const char * const str_cmp,
    bool *ptr_rslt
    );
extern bool
calc_is_next(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype, 
    cmp_t cmp
    );
